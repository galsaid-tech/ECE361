
#include "sensor.h"
#include "iom361_r4.h"

#include <math.h>
#include <stddef.h>

int sensor_read(uint32_t *base, time_t timestamp,
                reading_t *reading)
{
    int rc = 0;

    if (base == NULL || reading == NULL) {
        return -1;
    }

    /* Read temperature */
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);

    if (rc != 0) {
        return -1;
    }

    /* Read humidity */
    uint32_t raw_humid = iom361_readReg(base, HUMID_REG, &rc);

    if (rc != 0) {
        return -1;
    }

    /* Convert raw values */
    double temp = raw_temp / 1048576.0 * 200.0 - 50.0;
    double humid = raw_humid / 1048576.0 * 100.0;

    /* Round to one decimal place */
    reading->temperature = (float)(round(temp * 10.0) / 10.0);
    reading->humidity = (float)(round(humid * 10.0) / 10.0);

    /* Save the timestamp */
    reading->timestamp = timestamp;

    return 0;
}
