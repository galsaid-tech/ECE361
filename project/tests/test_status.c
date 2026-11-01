
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "bits.h"

int main(void)
{
    for (uint32_t state = 0; state < 4; state++) {
        uint32_t status = 0;

        int cooler = (state == 1);
        int humidifier = (state == 2);
        int alarm = (state == 3);

        status = set_field(status, 0, 2, state);
        status = set_field(status, 2, 1, cooler);
        status = set_field(status, 3, 1, humidifier);
        status = set_field(status, 4, 1, alarm);
        status = set_field(status, 5, 1, alarm);

        assert(get_field(status, 0, 2) == state);
        assert(get_field(status, 2, 1) == (uint32_t)cooler);
        assert(get_field(status, 3, 1) == (uint32_t)humidifier);
        assert(get_field(status, 4, 1) == (uint32_t)alarm);
        assert(get_field(status, 5, 1) == (uint32_t)alarm);

        /* Check unused bits */
        assert((status & ~0x3Fu) == 0);

        printf("State %u: status=0x%08X - PASSED\n",
               state, status);
    }

    printf("All status tests passed!\n");
    return 0;
}
