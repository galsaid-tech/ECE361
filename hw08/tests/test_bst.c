
#include "../bst.h"
#include <stdio.h>

static int failures = 0;

/* Print PASS or FAIL for each test */
static void check(int condition, const char *name)
{
    if (condition) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        failures++;
    }
}

int main(void)
{
    size_t visits = 0;

    /* Test 1: NULL tree */
    check(!bst_contains(NULL, 10, &visits) &&
          visits == 0, "NULL tree search");

    check(bst_size(NULL) == 0 &&
          bst_height(NULL) == 0,
          "NULL tree size and height");

    check(bst_insert(NULL, 10) == -1,
          "NULL tree insert");

    bst_destroy(NULL);
    check(1, "destroy NULL tree");

    /* Test 2: Empty tree */
    bst_t *empty = bst_create();

    if (empty == NULL) {
        printf("FAIL: create empty tree\n");
        return 1;
    }

    check(bst_size(empty) == 0 &&
          bst_height(empty) == 0,
          "empty tree size and height");

    visits = 99;
    check(!bst_contains(empty, 10, &visits) &&
          visits == 0,
          "empty tree search");

    bst_destroy(empty);

    /* Test 3: Original seven keys */
    bst_t *t = bst_create();

    if (t == NULL) {
        printf("FAIL: create tree\n");
        return 1;
    }

    int keys[] = {27, 14, 35, 10, 19, 31, 42};

    int inserted = 1;

    for (int i = 0; i < 7; i++) {
        if (bst_insert(t, keys[i]) != 0) {
            inserted = 0;
        }
    }

    check(inserted, "insert seven keys");
    check(bst_size(t) == 7, "size is 7");
    check(bst_height(t) == 3, "height is 3");

    visits = 0;
    check(bst_contains(t, 31, &visits) &&
          visits == 3,
          "find 31 in 3 visits");

    visits = 0;
    check(!bst_contains(t, 15, &visits) &&
          visits == 3,
          "15 not found in 3 visits");

    bst_destroy(t);
    check(1, "destroy full tree");

    /* Test 4: Duplicate keys */
    bst_t *dup = bst_create();

    if (dup == NULL) {
        printf("FAIL: create duplicate tree\n");
        return 1;
    }

    check(bst_insert(dup, 20) == 0 &&
          bst_insert(dup, 20) == 0,
          "insert duplicate keys");

    check(bst_size(dup) == 2,
          "duplicate size is 2");

    check(bst_height(dup) == 2,
          "duplicate height is 2");

    bst_destroy(dup);

    /* Test 5: Increasing order */
    bst_t *sorted = bst_create();

    if (sorted == NULL) {
        printf("FAIL: create sorted tree\n");
        return 1;
    }

    int sorted_ok = 1;

    for (int i = 1; i <= 10; i++) {
        if (bst_insert(sorted, i) != 0) {
            sorted_ok = 0;
        }
    }

    check(sorted_ok, "insert increasing keys");

    check(bst_size(sorted) == 10,
          "increasing tree size is 10");

    check(bst_height(sorted) == 10,
          "increasing tree height is 10");

    bst_destroy(sorted);

    /* Final result */
    if (failures == 0) {
        printf("\nAll tests passed!\n");
        return 0;
    }

    printf("\n%d test(s) failed.\n", failures);
    return 1;
}
