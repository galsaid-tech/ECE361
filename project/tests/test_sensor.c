
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "sensor.h"
#include "iom361_r4.h"

int main(void)
{
    int rc = 0;

    uint32_t *base = iom361_initialize(1, 3, &rc);
    assert(base != NULL && rc == 0);

    iom361_set_display(false);

    reading_t reading = {0};

    /* Test 1: 0 degrees */
    _iom361_set_sensor1(0.0f, 65.0f);

    assert(sensor_read(base, 100, &reading) == 0);
    assert(reading.temperature == 0.0f);

    printf("Test 1: 0 C passed\n");

    /* Test 2: 100 degrees */
    _iom361_set_sensor1(100.0f, 65.0f);

    assert(sensor_read(base, 101, &reading) == 0);
    assert(reading.temperature == 100.0f);

    printf("Test 2: 100 C passed\n");

    /* Test 3: Cooling threshold */
    _iom361_set_sensor1(14.0f, 60.0f);

    assert(sensor_read(base, 102, &reading) == 0);
    assert(reading.temperature == 14.0f);
    assert(reading.humidity == 60.0f);
    assert(reading.timestamp == (time_t)102);

    printf("Test 3: Threshold passed\n");

    printf("All sensor tests passed!\n");

    return 0;
}
