
#include "ht.h"
#include <stdlib.h>

/* One node stores a key and its count */
typedef struct node {
    int key;
    size_t count;
    struct node *next;
} node_t;

/* Hash table structure */
struct ht {
    size_t nbuckets;
    size_t size;
    node_t **buckets;
};

/* Create an empty hash table */
ht_t *ht_create(size_t nbuckets)
{
    if (nbuckets == 0) {
        return NULL;
    }

    ht_t *t = malloc(sizeof(*t));
    if (t == NULL) {
        return NULL;
    }

    t->buckets = calloc(nbuckets, sizeof(node_t *));
    if (t->buckets == NULL) {
        free(t);
        return NULL;
    }

    t->nbuckets = nbuckets;
    t->size = 0;

    return t;
}

/* Free all nodes and the table */
void ht_destroy(ht_t *t)
{
    if (t == NULL) {
        return;
    }

    for (size_t i = 0; i < t->nbuckets; i++) {
        node_t *current = t->buckets[i];

        while (current != NULL) {
            node_t *next = current->next;
            free(current);
            current = next;
        }
    }

    free(t->buckets);
    free(t);
}

/* Add a key or increase its count */
int ht_add(ht_t *t, int key)
{
    if (t == NULL) {
        return -1;
    }

    size_t index = (size_t)key % t->nbuckets;
    node_t *current = t->buckets[index];

    while (current != NULL) {
        if (current->key == key) {
            current->count++;
            return 0;
        }
        current = current->next;
    }

    node_t *new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return -1;
    }

    new_node->key = key;
    new_node->count = 1;
    new_node->next = t->buckets[index];

    t->buckets[index] = new_node;
    t->size++;

    return 0;
}

/* Return how many times a key was added */
size_t ht_count(const ht_t *t, int key)
{
    if (t == NULL) {
        return 0;
    }

    size_t index = (size_t)key % t->nbuckets;
    node_t *current = t->buckets[index];

    while (current != NULL) {
        if (current->key == key) {
            return current->count;
        }
        current = current->next;
    }

    return 0;
}

/* Return the number of different keys */
size_t ht_size(const ht_t *t)
{
    if (t == NULL) {
        return 0;
    }

    return t->size;
}

/* Find the longest chain */
size_t ht_longest_chain(const ht_t *t)
{
    if (t == NULL) {
        return 0;
    }

    size_t longest = 0;

    for (size_t i = 0; i < t->nbuckets; i++) {
        size_t length = 0;
        node_t *current = t->buckets[i];

        while (current != NULL) {
            length++;
            current = current->next;
        }

        if (length > longest) {
            longest = length;
        }
    }

    return longest;
}
