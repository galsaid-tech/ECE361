
#ifndef REPORT_H
#define REPORT_H

#include <time.h>
#include "store.h"

/* Stores the report results */
typedef struct {
    int count;
    time_t first_time;
    time_t last_time;
} report_result_t;

/* Find first, last, and total readings above 14 C */
int report_generate(const store_t *store,
                    report_result_t *result);

/* Print the report using ticks from the start */
void report_print(const report_result_t *result,
                  time_t start_time);

#endif
