
#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct bst bst_t;

/* Create an empty tree.
   Returns NULL if memory fails.
   Takes no input. */
bst_t *bst_create(void);

/* Free all nodes and the tree.
   NULL is allowed. */
void bst_destroy(bst_t *t);

/* Insert a key into the tree.
   Equal keys go right.
   Returns 0 on success, -1 on failure.
   Requires a valid tree. */
int bst_insert(bst_t *t, int key);

/* Search for a key.
   Returns true if found, false otherwise.
   Visits stores the number of nodes checked.
   NULL tree is allowed. */
bool bst_contains(const bst_t *t, int key, size_t *visits);

/* Return the number of keys.
   Returns 0 for NULL. */
size_t bst_size(const bst_t *t);

/* Return the longest path in nodes.
   Returns 0 for an empty or NULL tree. */
size_t bst_height(const bst_t *t);

#endif
