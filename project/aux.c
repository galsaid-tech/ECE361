
#include "aux.h"
#include <stdlib.h>

#define BUCKET_COUNT 3

/* One node in a linked list */
typedef struct aux_node {
    aux_entry_t entry;
    struct aux_node *next;
} aux_node_t;

/* Hidden hash table */
struct aux {
    aux_node_t *buckets[BUCKET_COUNT];
};

/* Find which bucket a state belongs to */
static unsigned int hash_state(state_t state)
{
    return (unsigned int)state % BUCKET_COUNT;
}

/* Create an empty table */
aux_t *aux_create(void)
{
    return calloc(1, sizeof(aux_t));
}

/* Find an existing state */
const aux_entry_t *aux_find(
    const aux_t *table, state_t state)
{
    if (table == NULL ||
        state < STABLE || state >= STATE_COUNT) {
        return NULL;
    }

    unsigned int index = hash_state(state);
    const aux_node_t *node = table->buckets[index];

    while (node != NULL) {
        if (node->entry.state == state) {
            return &node->entry;
        }

        node = node->next;
    }

    return NULL;
}

/* Insert a new state or update an existing one */
int aux_update(aux_t *table, state_t state,
               unsigned int ticks)
{
    if (table == NULL ||
        state < STABLE || state >= STATE_COUNT) {
        return -1;
    }

    unsigned int index = hash_state(state);
    aux_node_t *node = table->buckets[index];

    /* Check whether the state already exists */
    while (node != NULL) {
        if (node->entry.state == state) {
            node->entry.count++;
            node->entry.total_ticks += ticks;
            return 0;
        }

        node = node->next;
    }

    /* Create a new node */
    node = malloc(sizeof(*node));

    if (node == NULL) {
        return -1;
    }

    node->entry.state = state;
    node->entry.count = 1;
    node->entry.total_ticks = ticks;

    /* Add node at the start of the chain */
    node->next = table->buckets[index];
    table->buckets[index] = node;

    return 0;
}

/* Visit all entries */
void aux_walk(const aux_t *table,
              aux_callback_t callback,
              void *context)
{
    if (table == NULL || callback == NULL) {
        return;
    }

    for (int i = 0; i < BUCKET_COUNT; i++) {
        const aux_node_t *node = table->buckets[i];

        while (node != NULL) {
            callback(&node->entry, context);
            node = node->next;
        }
    }
}

/* Free the table and every node */
void aux_destroy(aux_t *table)
{
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < BUCKET_COUNT; i++) {
        aux_node_t *node = table->buckets[i];

        while (node != NULL) {
            aux_node_t *next = node->next;
            free(node);
            node = next;
        }
    }

    free(table);
}
