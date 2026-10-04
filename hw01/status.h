#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>

typedef struct
{
    int heat;
    int cool;
    int fan;
    int fault;
    int mode;
    int reserved;
    int32_t setpoint;

} status_t;

status_t status_unpack(uint16_t word);

#endif
