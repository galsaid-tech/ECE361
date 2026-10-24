
#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX 4
#define TALLY_NAMES 16
#define TALLY_LEN 15

typedef struct tally tally_t;

/* Creates an empty tally.
   Returns NULL if all 4 tallies are in use.
   Takes no arguments. */
tally_t *tally_create(void);

/* Frees a tally for reuse.
   NULL is allowed and does nothing. */
void tally_destroy(tally_t *t);

/* Adds a name or increases its count.
   Returns the new count, or -1 for invalid input.
   Assumes name is a valid C string when not NULL. */
int tally_add(tally_t *t, const char *name);

/* Gets the count of a name.
   Returns 0 if not found or an argument is NULL.
   Assumes name is a valid C string when not NULL. */
int tally_count(const tally_t *t, const char *name);

/* Gets the number of different names.
   Returns 0 if t is NULL. */
int tally_size(const tally_t *t);

/* Prints names and counts in alphabetical order.
   Assumes t and out are valid pointers. */
void tally_print(const tally_t *t, FILE *out);

#endif
