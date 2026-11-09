
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "sensor.h"
#include "panel.h"
#include "scenario.h"
#include "iom361_r4.h"

static int failures = 0;

#define CHECK(name, condition) do { \
    if (condition) { \
        printf("PASS: %s\n", name); \
    } else { \
        printf("FAIL: %s\n", name); \
        failures++; \
    } \
} while (0)

int main(void)
{
    int code = 0;
    uint32_t *base = iom361_initialize(4, 4, &code);
    iom361_set_display(false);

    if (base == NULL || code != 0) {
        printf("FAIL: initialization\n");
        return 1;
    }

    // Test 1: Sensor reads 91.0 C
    _iom361_set_sensor1(91.0f, 50.0f);

    reading_t r;
    code = sensor_read(base, (time_t)100, &r);

    CHECK("sensor 91 C",
          code == 0 && fabsf(r.temp_c - 91.0f) < 0.01f);

    CHECK("sensor humidity",
          fabsf(r.rh_pct - 50.0f) < 0.01f);

    CHECK("sensor timestamp", r.t == (time_t)100);

    // Test 2: Failed read changes nothing
    reading_t saved = {123, 25.0f, 60.0f};
    reading_t before = saved;

    code = sensor_read(NULL, (time_t)999, &saved);

    CHECK("failed sensor read",
          code != 0 &&
          saved.t == before.t &&
          saved.temp_c == before.temp_c &&
          saved.rh_pct == before.rh_pct);

    // Test 3: Switch press, hold, and release
    CHECK("switch pressed",
          panel_pressed(0x1, 0x0) == 0x1);

    CHECK("switch held",
          panel_pressed(0x1, 0x1) == 0x0);

    CHECK("switch released",
          panel_released(0x0, 0x1) == 0x1);

    CHECK("switch not released",
          panel_released(0x1, 0x1) == 0x0);

    // Test 4: LEDs keep other bits unchanged
    iom361_writeReg(base, LEDS_REG, 0x5, &code);

    code = panel_led(base, 1, 1);
    uint32_t leds = iom361_readReg(base, LEDS_REG, &code);

    CHECK("LED on keeps others",
          leds == 0x7);

    code = panel_led(base, 1, 0);
    leds = iom361_readReg(base, LEDS_REG, &code);

    CHECK("LED off keeps others",
          leds == 0x5);

    code = panel_toggle(base, 0);
    leds = iom361_readReg(base, LEDS_REG, &code);

    CHECK("LED toggle keeps others",
          leds == 0x4);

    CHECK("invalid LED number",
          panel_led(base, 32, 1) == -1 &&
          panel_toggle(base, -1) == -1);

    // Test 5: Load scenario with comments
    scenario_t s;
    code = scenario_load("scenarios/test1.txt", &s);

    CHECK("load scenario with comments",
          code == 0 && s.count == 5);

    if (code == 0) {
        const scenario_line_t *line = scenario_at(&s, 3);

        CHECK("values hold between lines",
              line != NULL &&
              line->tick == 2 &&
              line->switches == 0x1);
    }

    // Test 6: Invalid scenario files
    FILE *f = fopen("tests/invalid.txt", "w");

    if (f == NULL) {
        printf("FAIL: create invalid test file\n");
        return 1;
    }

    fputs("0 20.0 50.0 0x0\n", f);
    fclose(f);

    CHECK("missing start line",
          scenario_load("tests/invalid.txt", &s) != 0);

    f = fopen("tests/invalid.txt", "w");
    if (f == NULL) return 1;

    fputs("start 100\n0 20.0 50.0\n", f);
    fclose(f);

    CHECK("missing field",
          scenario_load("tests/invalid.txt", &s) == 2);

    f = fopen("tests/invalid.txt", "w");
    if (f == NULL) return 1;

    fputs("start 100\n2 20.0 50.0 0x0\n"
          "1 21.0 55.0 0x1\n", f);
    fclose(f);

    CHECK("ticks not increasing",
          scenario_load("tests/invalid.txt", &s) == 3);

    remove("tests/invalid.txt");

    if (failures != 0) {
        printf("%d test(s) failed.\n", failures);
        return 1;
    }

    printf("All unit tests passed.\n");
    return 0;
}
