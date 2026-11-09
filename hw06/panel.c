
#include "panel.h"
#include "iom361_r4.h"

uint32_t panel_pressed(uint32_t now, uint32_t before)
{
    return now & ~before;
}

uint32_t panel_released(uint32_t now, uint32_t before)
{
    return before & ~now;
}

int panel_led(uint32_t *base, int n, int on)
{
    if (n < 0 || n > 31) {
        return -1;
    }

    int code = 0;

    uint32_t leds =
        iom361_readReg(base, LEDS_REG, &code);

    if (code != 0) {
        return code;
    }

    uint32_t mask = 1u << n;

    if (on) {
        leds |= mask;
    } else {
        leds &= ~mask;
    }

    iom361_writeReg(base, LEDS_REG, leds, &code);

    return code;
}

int panel_toggle(uint32_t *base, int n)
{
    if (n < 0 || n > 31) {
        return -1;
    }

    int code = 0;

    uint32_t leds =
        iom361_readReg(base, LEDS_REG, &code);

    if (code != 0) {
        return code;
    }

    leds ^= (1u << n);

    iom361_writeReg(base, LEDS_REG, leds, &code);

    return code;
}
