
#include <stdio.h>
#include <assert.h>
#include "aux.h"

/* Count entries during a walk */
static void count_entry(const aux_entry_t *entry,
                        void *context)
{
    int *count = context;

    assert(entry != NULL);
    (*count)++;
}

int main(void)
{
    aux_t *table = aux_create();
    assert(table != NULL);

    /* Test 1: State not found */
    assert(aux_find(table, STABLE) == NULL);
    printf("Test 1: Missing state - PASSED\n");

    /* Test 2: Insert a state */
    assert(aux_update(table, STABLE, 1) == 0);

    const aux_entry_t *entry =
        aux_find(table, STABLE);

    assert(entry != NULL);
    assert(entry->count == 1);
    assert(entry->total_ticks == 1);

    printf("Test 2: Insert state - PASSED\n");

    /* Test 3: Update an existing state */
    assert(aux_update(table, STABLE, 1) == 0);

    entry = aux_find(table, STABLE);
    assert(entry->count == 2);
    assert(entry->total_ticks == 2);

    printf("Test 3: Update state - PASSED\n");

    /* Test 4: Collision in bucket 0 */
    assert(aux_update(table, ALARM, 1) == 0);

    assert(aux_find(table, STABLE) != NULL);
    assert(aux_find(table, ALARM) != NULL);

    printf("Test 4: Collision - PASSED\n");

    /* Test 5: Insert all four states */
    assert(aux_update(table, COOLING, 3) == 0);
    assert(aux_update(table, HUMIDIFY, 2) == 0);

    entry = aux_find(table, COOLING);
    assert(entry != NULL);
    assert(entry->count == 1);
    assert(entry->total_ticks == 3);

    printf("Test 5: All states - PASSED\n");

    /* Test 6: Visit all entries */
    int count = 0;
    aux_walk(table, count_entry, &count);

    assert(count == 4);

    printf("Test 6: Visit entries - PASSED\n");

    /* Test 7: Invalid input */
    assert(aux_update(NULL, STABLE, 1) == -1);
    assert(aux_update(table, STATE_COUNT, 1) == -1);
    assert(aux_find(table, STATE_COUNT) == NULL);

    printf("Test 7: Invalid input - PASSED\n");

    /* Free table with chains */
    aux_destroy(table);

    printf("Test 8: Destroy table - PASSED\n");
    printf("All 8 AUX tests passed!\n");

    return 0;
}
