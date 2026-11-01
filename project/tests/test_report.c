
#include <stdio.h>
#include <assert.h>
#include "report.h"

/* Make a record with temperature and time */
static record_t make_record(float temperature,
                            time_t timestamp)
{
    record_t record = {0};

    record.reading.temperature = temperature;
    record.reading.timestamp = timestamp;

    return record;
}

/* Test 1: No readings above 14 C */
static void test_no_match(void)
{
    store_t *store = store_create();
    assert(store != NULL);

    assert(store_insert(store,
           make_record(13.0f, 100)) == 0);

    assert(store_insert(store,
           make_record(14.0f, 101)) == 0);

    report_result_t result;

    assert(report_generate(store, &result) == 0);
    assert(result.count == 0);

    printf("Test 1: No matching readings - PASSED\n");

    store_destroy(store);
}

/* Test 2: Exactly one matching reading */
static void test_one_match(void)
{
    store_t *store = store_create();
    assert(store != NULL);

    assert(store_insert(store,
           make_record(15.0f, 105)) == 0);

    report_result_t result;

    assert(report_generate(store, &result) == 0);
    assert(result.count == 1);
    assert(result.first_time == (time_t)105);
    assert(result.last_time == (time_t)105);

    printf("Test 2: One matching reading - PASSED\n");

    store_destroy(store);
}

/* Test 3: First and last readings match */
static void test_first_last(void)
{
    store_t *store = store_create();
    assert(store != NULL);

    assert(store_insert(store,
           make_record(15.0f, 100)) == 0);

    assert(store_insert(store,
           make_record(13.0f, 101)) == 0);

    assert(store_insert(store,
           make_record(14.5f, 102)) == 0);

    assert(store_insert(store,
           make_record(14.0f, 103)) == 0);

    assert(store_insert(store,
           make_record(16.0f, 104)) == 0);

    report_result_t result;

    assert(report_generate(store, &result) == 0);
    assert(result.count == 3);
    assert(result.first_time == (time_t)100);
    assert(result.last_time == (time_t)104);

    printf("Test 3: First and last - PASSED\n");

    store_destroy(store);
}

/* Test 4: Records not in time order */
static void test_time_order(void)
{
    store_t *store = store_create();
    assert(store != NULL);

    assert(store_insert(store,
           make_record(16.0f, 120)) == 0);

    assert(store_insert(store,
           make_record(14.5f, 100)) == 0);

    assert(store_insert(store,
           make_record(15.0f, 110)) == 0);

    report_result_t result;

    assert(report_generate(store, &result) == 0);
    assert(result.count == 3);
    assert(result.first_time == (time_t)100);
    assert(result.last_time == (time_t)120);

    printf("Test 4: Time order - PASSED\n");

    store_destroy(store);
}

/* Test 5: Invalid inputs */
static void test_invalid(void)
{
    store_t *store = store_create();
    assert(store != NULL);

    report_result_t result;

    assert(report_generate(NULL, &result) == -1);
    assert(report_generate(store, NULL) == -1);

    printf("Test 5: Invalid inputs - PASSED\n");

    store_destroy(store);
}

int main(void)
{
    test_no_match();
    test_one_match();
    test_first_last();
    test_time_order();
    test_invalid();

    printf("All 5 report tests passed!\n");

    return 0;
}
