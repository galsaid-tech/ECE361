
#include <stdio.h>
#include <assert.h>
#include "store.h"

/* Count records visited during the walk */
typedef struct {
    int count;
    float previous;
    int sorted;
    int duplicates;
} test_data_t;

/* Check records during the tree walk */
void check_record(const record_t *record, void *context)
{
    test_data_t *data = context;
    float temp = record->reading.temperature;

    if (data->count > 0 && temp < data->previous) {
        data->sorted = 0;
    }

    if (temp == 13.0f || temp == 13.2f) {
        data->duplicates++;
    }

    data->previous = temp;
    data->count++;
}

/* Test temperature comparisons */
void test_compare(void)
{
    record_t a = {0};
    record_t b = {0};
    record_t c = {0};
    record_t d = {0};
    record_t e = {0};
    record_t f = {0};

    a.reading.temperature = 13.0f;
    b.reading.temperature = 13.2f;
    c.reading.temperature = 14.0f;
    d.reading.temperature = 13.4f;
    e.reading.temperature = 13.5f;
    f.reading.temperature = -0.2f;

    /* Same 0.5 C step */
    assert(record_compare(&a, &b) == 0);
    assert(record_compare(&a, &d) == 0);

    /* Different steps */
    assert(record_compare(&a, &e) < 0);
    assert(record_compare(&a, &c) < 0);
    assert(record_compare(&c, &a) > 0);

    /* Negative temperature */
    assert(record_compare(&f, &a) < 0);

    printf("Test 1: Temperature keys - PASSED\n");
}

/* Test an empty tree */
void test_empty_tree(void)
{
    store_t *s = store_create();
    assert(s != NULL);

    assert(store_count(s) == 0);
    assert(store_height(s) == 0);
    assert(store_find(s, 13.0f) == NULL);

    store_destroy(s);

    printf("Test 2: Empty tree - PASSED\n");
}

/* Test inserting one record */
void test_one_record(void)
{
    store_t *s = store_create();
    assert(s != NULL);

    record_t a = {0};
    a.reading.temperature = 13.0f;

    assert(store_insert(s, a) == 0);
    assert(store_count(s) == 1);
    assert(store_height(s) == 1);

    const record_t *found = store_find(s, 13.0f);
    assert(found != NULL);
    assert(found->reading.temperature == 13.0f);

    store_destroy(s);

    printf("Test 3: One record - PASSED\n");
}

/* Test two records */
void test_two_records(void)
{
    store_t *s = store_create();
    assert(s != NULL);

    record_t a = {0};
    record_t b = {0};

    a.reading.temperature = 13.0f;
    b.reading.temperature = 14.0f;

    assert(store_insert(s, a) == 0);
    assert(store_insert(s, b) == 0);

    assert(store_count(s) == 2);
    assert(store_height(s) == 2);

    store_destroy(s);

    printf("Test 4: Two records - PASSED\n");
}

/* Test duplicate keys and sorted order */
void test_duplicates(void)
{
    store_t *s = store_create();
    assert(s != NULL);

    record_t a = {0};
    record_t b = {0};
    record_t c = {0};

    a.reading.temperature = 13.0f;
    b.reading.temperature = 13.2f;
    c.reading.temperature = 14.0f;

    assert(store_insert(s, c) == 0);
    assert(store_insert(s, a) == 0);
    assert(store_insert(s, b) == 0);

    assert(store_count(s) == 3);
    assert(store_height(s) == 2);

    /* Find a record with a matching key */
    const record_t *found = store_find(s, 13.0f);
    assert(found != NULL);
    assert(found->reading.temperature == 13.0f);

    /* A different value in the same step */
    found = store_find(s, 13.2f);
    assert(found != NULL);

    /* Missing key */
    assert(store_find(s, 20.0f) == NULL);

    test_data_t data = {0};
    data.sorted = 1;

    store_walk(s, check_record, &data);

    assert(data.count == 3);
    assert(data.sorted == 1);
    assert(data.duplicates == 2);

    store_destroy(s);

    printf("Test 5: Duplicates and order - PASSED\n");
}

/* Test inserting 100 records */
void test_100_records(void)
{
    store_t *s = store_create();
    assert(s != NULL);

    for (int i = 0; i < 100; i++) {
        record_t r = {0};

        r.reading.temperature = (float)i;

        assert(store_insert(s, r) == 0);
    }

    assert(store_count(s) == 100);

    /* Increasing values create a chain */
    assert(store_height(s) == 100);

    assert(store_find(s, 0.0f) != NULL);
    assert(store_find(s, 99.0f) != NULL);
    assert(store_find(s, 150.0f) == NULL);

    store_destroy(s);

    printf("Test 6: 100 records - PASSED\n");
}

int main(void)
{
    test_compare();
    test_empty_tree();
    test_one_record();
    test_two_records();
    test_duplicates();
    test_100_records();

    printf("All 6 BST tests passed!\n");

    return 0;
}
