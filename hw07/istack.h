#ifndef ISTACK_H
#define ISTACK_H

#include <stdbool.h>
#include <stddef.h>

#define ISTACK_START 8  /* Room at first; doubles when full. */
typedef struct istack istack_t; /* Opaque: details are in istack.c. */

/* Takes: nothing. Returns: a new empty stack, or NULL if allocation fails.
   Assumes: the caller destroys a successful stack. */
istack_t *istack_create(void);

/* Takes: a stack (or NULL). Returns: nothing.
   Assumes: s has not already been destroyed; NULL is safe. */
void istack_destroy(istack_t *s);

/* Takes: a stack and an integer. Returns: 0 on success, -1 on failure.
   Assumes: s is live; a NULL stack fails; failed growth keeps old data. */
int istack_push(istack_t *s, int v);

/* Takes: a stack and optional output pointer. Returns: 0 on success, -1
   if NULL/empty. Assumes: if v is not NULL, it points to a writable int. */
int istack_pop(istack_t *s, int *v);

/* Takes: a stack and optional output pointer. Returns: 0 on success, -1
   if NULL/empty. Assumes: if v is not NULL, it points to a writable int. */
int istack_peek(const istack_t *s, int *v);

/* Takes: a stack (or NULL). Returns: its number of values, or 0 for NULL.
   Assumes: s is live if not NULL. */
size_t istack_count(const istack_t *s);

/* Takes: a stack (or NULL). Returns: true if empty/NULL, else false.
   Assumes: s is live if not NULL. */
bool istack_is_empty(const istack_t *s);

/* Takes: a stack (or NULL). Returns: its allocated room, or 0 for NULL.
   Assumes: s is live if not NULL. */
size_t istack_capacity(const istack_t *s);

/* Takes: a stack and optional size pointer n. Returns: a new, independent
   bottom-first array, or NULL for empty/NULL/failure; writes its length to
   *n if n is not NULL. Assumes: the caller frees a successful result. */
int *istack_to_array(const istack_t *s, size_t *n);

#endif
