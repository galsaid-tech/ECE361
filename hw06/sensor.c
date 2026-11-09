
#include "sensor.h"
#include "iom361_r4.h"
#include <math.h>

int sensor_read(uint32_t *base, time_t now, reading_t *r)
{
    if (r == NULL) {
        return 1;
    }

    int code = 0;

    uint32_t temp_raw =
        iom361_readReg(base, TEMP_REG, &code);

    if (code != 0) {
        return code;
    }

    uint32_t humid_raw =
        iom361_readReg(base, HUMID_REG, &code);

    if (code != 0) {
        return code;
    }

    // Keep only the lowest 20 bits
    temp_raw &= 0xFFFFF;
    humid_raw &= 0xFFFFF;

    // Convert raw values
    float temp = ((float)temp_raw / 1048576.0f)
                 * 200.0f - 50.0f;

    float humid = ((float)humid_raw / 1048576.0f)
                  * 100.0f;

    // Round to one decimal place
    temp = roundf(temp * 10.0f) / 10.0f;
    humid = roundf(humid * 10.0f) / 10.0f;

    // Save results only after both reads succeed
    r->t = now;
    r->temp_c = temp;
    r->rh_pct = humid;

    return 0;
}
