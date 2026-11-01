
#include <stdio.h>
#include "check.h"

static int passed = 0;
static int failed = 0;

/* Print PASS or FAIL. */
static void test(int condition, const char *name)
{
    if (condition) {
        printf("PASS: %s\n", name);
        passed++;
    } else {
        printf("FAIL: %s\n", name);
        failed++;
    }
}

/* Copy the original table. */
static void copy_table(
    transition_t dest[N_STATES][N_EVENTS])
{
    for (int s = 0; s < N_STATES; s++) {
        for (int e = 0; e < N_EVENTS; e++) {
            dest[s][e] = lock_table[s][e];
        }
    }
}

int main(void)
{
    transition_t table[N_STATES][N_EVENTS];

    /* Test 1: Correct table. */
    copy_table(table);
    int result = fsm_check(table, LOCKED, stdout);
    test(result == 0, "correct table");

    /* Test 2: Missing table entry. */
    copy_table(table);
    table[GOT1][E_RIGHT].action = A_UNDEFINED;
    result = fsm_check(table, LOCKED, stdout);
    test(result > 0, "missing entry");

    /* Test 3: Invalid next state. */
    copy_table(table);
    table[GOT2][E_RIGHT].next = N_STATES;
    result = fsm_check(table, LOCKED, stdout);
    test(result > 0, "invalid next state");

    /* Test 4: Unreachable state. */
    copy_table(table);
    table[LOCKED][E_RIGHT].next = LOCKED;
    result = fsm_check(table, LOCKED, stdout);
    test(result > 0, "unreachable state");

    /* Test 5: State with no way out. */
    copy_table(table);
    for (int e = 0; e < N_EVENTS; e++) {
        table[GOT1][e].next = GOT1;
    }
    result = fsm_check(table, LOCKED, stdout);
    test(result > 0, "no way out");

    printf("\nBonus summary: %d passed, %d failed\n",
           passed, failed);

    return failed == 0 ? 0 : 1;
}
