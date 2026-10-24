
#ifndef STORE_H
#define STORE_H

#include <stdint.h>
#include "sensor.h"

/*
 * This module saves sensor readings.
 *
 * For now, we use a sorted array.
 * In Week 8, we will use a Binary Search Tree.
 */

/* Stores one record */
typedef struct {
    reading_t reading;
    int state;
    uint32_t status;
} record_t;

/* Opaque type */
typedef struct store store_t;

/* Creates an empty store */
store_t *store_create(void);

/* Adds a record */
int store_insert(store_t *s, record_t record);

/* Returns the number of records */
int store_count(const store_t *s);

/* Releases the store */
void store_destroy(store_t *s);

/* Compares two records by temperature key */
int record_compare(const record_t *a, const record_t *b);

/* Finds a record using a temperature key.
   Returns NULL if the record is not found. */
const record_t *store_find(const store_t *s, float temperature);


/* Visits one record */
typedef void (*store_callback_t)(const record_t *record, void *context);

/* Visits all records in sorted order */
void store_walk(const store_t *s,
                store_callback_t callback,
                void *context);

#endif
