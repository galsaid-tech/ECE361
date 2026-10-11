
#include <stdio.h>
#include "stats.h"

/* Check one test and return 1 for PASS or 0 for FAIL. */
static int check_test(const char *name, int condition)
{
    if (condition) {
        printf("PASS %s\n", name);
        return 1;
    }

    printf("FAIL %s\n", name);
    return 0;
}

int main(void)
{
    int tests_run = 0;
    int tests_passed = 0;

    /* Test 1: One value. */
    float one[] = {5.0f};

    tests_run++;
    tests_passed += check_test("one value",
        stats_min(one, 1) == 5.0f &&
        stats_max(one, 1) == 5.0f &&
        stats_mean(one, 1) == 5.0f);

    /* Test 2: Maximum at the start and end. */
    float first[] = {9.0f, 2.0f, 4.0f};
    float last[] = {2.0f, 4.0f, 9.0f};

    tests_run++;
    tests_passed += check_test("maximum at start and end",
        stats_max(first, 3) == 9.0f &&
        stats_max(last, 3) == 9.0f);

    /* Test 3: Equal to threshold ends a run. */
    float values[] = {31.0f, 32.0f, 30.0f, 33.0f};
    int best_start;

    int best = stats_longest_run_above(values, 4,
                                       30.0f, &best_start);

    tests_run++;
    tests_passed += check_test("value equal to threshold",
        best == 2 && best_start == 0);

    /* Test 4: Recursive maximum with 10000 values. */
    float large[10000];

    for (int i = 0; i < 10000; i++) {
        large[i] = (float)i;
    }

    tests_run++;
    tests_passed += check_test("recursive max 10000 values",
        stats_max(large, 10000) == 9999.0f);

    printf("\nSummary: %d/%d tests passed\n",
           tests_passed, tests_run);

    if (tests_passed == tests_run)
        return 0;

    return 1;
}
