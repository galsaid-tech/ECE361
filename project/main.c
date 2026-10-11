
#include <stdio.h>
#include <stdint.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "fsm.h"
#include "store.h"
#include "aux.h"
#include "report.h"

int main(void)
{
    int code = 0;

    uint32_t *base = iom361_initialize(1, 3, &code);

    if (base == NULL || code != 0) {
        printf("Initialization failed.\n");
        return 1;
    }

    printf("Wine Cellar Monitor\n");
    printf("Project setup is ready.\n");

    return 0;
}
