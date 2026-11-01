
#include "report.h"
#include <stdio.h>
#include <stddef.h>

/* Check one stored record */
static void check_record(const record_t *record,
                         void *context)
{
    report_result_t *result = context;

    /* Only temperatures above 14.0 C count */
    if (record->reading.temperature <= 14.0f) {
        return;
    }

    time_t timestamp = record->reading.timestamp;

    /* First matching reading */
    if (result->count == 0) {
        result->first_time = timestamp;
        result->last_time = timestamp;
    }
    else {
        /* Find the earliest time */
        if (timestamp < result->first_time) {
            result->first_time = timestamp;
        }

        /* Find the latest time */
        if (timestamp > result->last_time) {
            result->last_time = timestamp;
        }
    }

    result->count++;
}

/* Generate the report from stored records */
int report_generate(const store_t *store,
                    report_result_t *result)
{
    if (store == NULL || result == NULL) {
        return -1;
    }

    result->count = 0;
    result->first_time = 0;
    result->last_time = 0;

    /* Visit all records using store.h */
    store_walk(store, check_record, result);

    return 0;
}

/* Print the report */
void report_print(const report_result_t *result,
                  time_t start_time)
{
    if (result == NULL) {
        return;
    }

    if (result->count == 0) {
        printf("No readings above 14.0 C.\n");
        return;
    }

    /* Convert timestamps into scenario ticks */
    long long first_tick =
        (long long)difftime(result->first_time,
                           start_time);

    long long last_tick =
        (long long)difftime(result->last_time,
                           start_time);

    printf("Temperature above 14.0 C report:\n");
    printf("First tick: %lld\n", first_tick);
    printf("Last tick: %lld\n", last_tick);
    printf("Total readings: %d\n", result->count);
}
