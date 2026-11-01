
#ifndef AUX_H
#define AUX_H

#include "fsm.h"

/* Hidden hash table structure */
typedef struct aux aux_t;

/* Information for one state */
typedef struct {
    state_t state;
    unsigned int count;
    unsigned long long total_ticks;
} aux_entry_t;

/* Callback for visiting entries */
typedef void (*aux_callback_t)(
    const aux_entry_t *entry,
    void *context);

/* Create an empty hash table */
aux_t *aux_create(void);

/* Add or update a state */
int aux_update(aux_t *table, state_t state,
               unsigned int ticks);

/* Find a state */
const aux_entry_t *aux_find(
    const aux_t *table, state_t state);

/* Visit every entry */
void aux_walk(const aux_t *table,
              aux_callback_t callback,
              void *context);

/* Free the hash table */
void aux_destroy(aux_t *table);

#endif
