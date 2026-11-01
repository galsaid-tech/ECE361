
#ifndef STORE_H
#define STORE_H

#include <stdint.h>
#include "sensor.h"

/* Stores one sensor record */
typedef struct {
    reading_t reading;
    int state;
    uint32_t status;
} record_t;

/* Hidden store structure */
typedef struct store store_t;

/* Function used when visiting records */
typedef void (*store_callback_t)(
    const record_t *record, void *context);

/* Create an empty store */
store_t *store_create(void);

/* Add a record */
int store_insert(store_t *s, record_t record);

/* Count all records */
int store_count(const store_t *s);

/* Find a record by temperature key */
const record_t *store_find(
    const store_t *s, float temperature);

/* Compare two records */
int record_compare(
    const record_t *a, const record_t *b);

/* Visit all records in sorted order */
void store_walk(
    const store_t *s,
    store_callback_t callback,
    void *context);

/* Get the height of the tree */
int store_height(const store_t *s);

/* Free the tree */
void store_destroy(store_t *s);

#endif
