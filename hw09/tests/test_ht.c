
#include "../ht.h"
#include <stdio.h>
#include <stdlib.h>

/* Count failed tests */
static int failures = 0;

/* Print PASS or FAIL */
static void check(int condition, const char *name)
{
    if (condition) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        failures++;
    }
}

/* Test creating a table with zero buckets */
static void test_zero_buckets(void)
{
    ht_t *t = ht_create(0);
    check(t == NULL, "Zero buckets");
    ht_destroy(t);
}

/* Test an empty table */
static void test_empty(void)
{
    ht_t *t = ht_create(10);

    if (t == NULL) {
        check(0, "Empty table");
        return;
    }

    check(ht_size(t) == 0 &&
          ht_count(t, 5) == 0 &&
          ht_longest_chain(t) == 0,
          "Empty table");

    ht_destroy(t);
}

/* Test adding the same key three times */
static void test_repeated_key(void)
{
    ht_t *t = ht_create(10);

    if (t == NULL) {
        check(0, "Repeated key");
        return;
    }

    int ok = ht_add(t, 5) == 0 &&
             ht_add(t, 5) == 0 &&
             ht_add(t, 5) == 0;

    check(ok &&
          ht_count(t, 5) == 3 &&
          ht_size(t) == 1 &&
          ht_longest_chain(t) == 1,
          "Repeated key");

    ht_destroy(t);
}

/* Test keys that share the same bucket */
static void test_collision(void)
{
    ht_t *t = ht_create(10);

    if (t == NULL) {
        check(0, "Collision");
        return;
    }

    int ok = ht_add(t, 5) == 0 &&
             ht_add(t, 15) == 0;

    check(ok &&
          ht_count(t, 5) == 1 &&
          ht_count(t, 15) == 1 &&
          ht_count(t, 25) == 0 &&
          ht_size(t) == 2 &&
          ht_longest_chain(t) == 2,
          "Collision");

    ht_destroy(t);
}

/* Test 1 to 1000 */
static void test_1000_keys(void)
{
    ht_t *t = ht_create(1000);

    if (t == NULL) {
        check(0, "1000 keys");
        return;
    }

    int ok = 1;

    for (int i = 1; i <= 1000; i++) {
        if (ht_add(t, i) != 0) {
            ok = 0;
        }
    }

    for (int i = 1; i <= 1000; i++) {
        if (ht_count(t, i) != 1) {
            ok = 0;
        }
    }

    check(ok && ht_size(t) == 1000,
          "1000 keys");

    ht_destroy(t);
}

/* Test longest chain for different bucket counts */
static void test_chain(size_t buckets,
                       int step,
                       size_t expected,
                       const char *name)
{
    ht_t *t = ht_create(buckets);

    if (t == NULL) {
        check(0, name);
        return;
    }

    int ok = 1;

    for (int i = 1; i <= 1000; i++) {
        if (ht_add(t, i * step) != 0) {
            ok = 0;
        }
    }

    check(ok &&
          ht_size(t) == 1000 &&
          ht_longest_chain(t) == expected,
          name);

    ht_destroy(t);
}

/* Test NULL arguments */
static void test_null(void)
{
    check(ht_add(NULL, 5) == -1 &&
          ht_count(NULL, 5) == 0 &&
          ht_size(NULL) == 0 &&
          ht_longest_chain(NULL) == 0,
          "NULL arguments");

    ht_destroy(NULL);
    check(1, "Destroy NULL");
}

/* Run all tests */
int main(void)
{
    test_zero_buckets();
    test_empty();
    test_repeated_key();
    test_collision();
    test_1000_keys();

    test_chain(10, 1, 100,
               "Chain: 10 buckets");

    test_chain(1000, 1, 1,
               "Chain: 1000 buckets");

    test_chain(1000, 10, 10,
               "Chain: multiples of 10");

    test_chain(1009, 10, 1,
               "Chain: 1009 buckets");

    test_null();

    if (failures == 0) {
        printf("\nAll tests passed!\n");
        return 0;
    }

    printf("\n%d test(s) failed.\n", failures);
    return 1;
}
