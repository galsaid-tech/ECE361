
#ifndef HT_H
#define HT_H

#include <stddef.h>

/* Hash table type (hidden structure) */
typedef struct ht ht_t;

/* Creates an empty hash table.
   Takes: number of buckets.
   Returns: new table or NULL if it fails.
   Assumes: no special conditions. */
ht_t *ht_create(size_t nbuckets);

/* Frees all memory used by the table.
   Takes: a table pointer.
   Returns: nothing.
   Assumes: NULL is allowed. */
void ht_destroy(ht_t *t);

/* Adds a key or increases its count.
   Takes: a table and a non-negative key.
   Returns: 0 on success, -1 on failure.
   Assumes: key is not negative. */
int ht_add(ht_t *t, int key);

/* Finds how many times a key was added.
   Takes: a table and a non-negative key.
   Returns: key count or 0 if not found.
   Assumes: key is not negative. */
size_t ht_count(const ht_t *t, int key);

/* Counts different keys in the table.
   Takes: a table.
   Returns: number of different keys, or 0 for NULL.
   Assumes: NULL is allowed. */
size_t ht_size(const ht_t *t);

/* Finds the longest linked list.
   Takes: a table.
   Returns: number of nodes in the longest chain.
   Assumes: NULL is allowed. */
size_t ht_longest_chain(const ht_t *t);

#endif
