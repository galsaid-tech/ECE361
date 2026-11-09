
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

int main(int argc, char *argv[])
{
    // Check the command
    if (argc != 2) {
        fprintf(stderr, "Usage: ./replay scenario.txt\n");
        return 1;
    }

    // Load the scenario
    scenario_t scenario;

    int error = scenario_load(argv[1], &scenario);

    if (error != 0) {
        fprintf(stderr,
                "Scenario error at line %d\n", error);
        return 1;
    }

    // Initialize 4 switches and 4 LEDs
    int code = 0;
    uint32_t *base = iom361_initialize(4, 4, &code);

    if (base == NULL || code != 0) {
        fprintf(stderr, "Initialization failed\n");
        return 1;
    }

    // Turn off the LED display output
    iom361_set_display(false);

    uint32_t before = 0;

    // Run from tick 0 to the last tick
    int last_tick =
        scenario.lines[scenario.count - 1].tick;

    for (int tick = 0; tick <= last_tick; tick++) {

        // Get scenario values for this tick
        const scenario_line_t *line =
            scenario_at(&scenario, tick);

        if (line == NULL) {
            fprintf(stderr,
                    "No scenario values at tick %d\n", tick);
            return 1;
        }

        // Update simulated sensor and switches
        _iom361_set_sensor1(
            line->temp_c, line->rh_pct);

        _iom361_set_switches(line->switches);

        // Calculate current time
        time_t now = scenario.start + tick;

        // Read sensor through registers
        reading_t reading;

        code = sensor_read(base, now, &reading);

        if (code != 0) {
            fprintf(stderr, "Sensor read failed\n");
            return 1;
        }

        // Read switches through register
        uint32_t switches =
            iom361_readReg(base, SWITCHES_REG, &code);

        if (code != 0) {
            fprintf(stderr, "Switch read failed\n");
            return 1;
        }

        // Find switches just pressed
        uint32_t pressed =
            panel_pressed(switches, before);

        // Toggle LED for each new press
        for (int k = 0; k < 4; k++) {
            if (pressed & (1u << k)) {
                code = panel_toggle(base, k);

                if (code != 0) {
                    fprintf(stderr, "LED toggle failed\n");
                    return 1;
                }
            }
        }

        // Read current LED states
        uint32_t leds =
            iom361_readReg(base, LEDS_REG, &code);

        if (code != 0) {
            fprintf(stderr, "LED read failed\n");
            return 1;
        }

        // Format time in UTC
        char time_text[32];

        struct tm *utc = gmtime(&reading.t);

        if (utc == NULL ||
            strftime(time_text, sizeof(time_text),
                     "%Y-%m-%d %H:%M:%S", utc) == 0) {
            fprintf(stderr, "Time format failed\n");
            return 1;
        }

        // Print the trace
        printf("%s temp=%.1f rh=%.1f "
               "switches=0x%x pressed=0x%x leds=0x%x\n",
               time_text,
               reading.temp_c,
               reading.rh_pct,
               (unsigned int)switches,
               (unsigned int)pressed,
               (unsigned int)leds);

        // Save switch state for next tick
        before = switches;
    }

    return 0;
}
