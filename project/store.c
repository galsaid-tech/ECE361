
#include "store.h"
#include <stdlib.h>
#include <math.h>

/* List for records with the same key */
typedef struct record_node {
    record_t record;
    struct record_node *next;
} record_node_t;

/* One node in the binary search tree */
typedef struct tree_node {
    int key;
    record_node_t *records;
    record_node_t *last;
    struct tree_node *left;
    struct tree_node *right;
} tree_node_t;

/* Hidden store structure */
struct store {
    tree_node_t *root;
    int count;
};

/* Convert temperature to 0.5 C key */
static int temperature_key(float temperature)
{
    return (int)floorf(temperature * 2.0f);
}

/* Compare two records */
int record_compare(const record_t *a,
                   const record_t *b)
{
    if (a == NULL || b == NULL) {
        return 0;
    }

    int key_a = temperature_key(
        a->reading.temperature);
    int key_b = temperature_key(
        b->reading.temperature);

    if (key_a < key_b) {
        return -1;
    }

    if (key_a > key_b) {
        return 1;
    }

    return 0;
}

/* Create an empty store */
store_t *store_create(void)
{
    store_t *s = malloc(sizeof(store_t));

    if (s == NULL) {
        return NULL;
    }

    s->root = NULL;
    s->count = 0;

    return s;
}

/* Create a record list item */
static record_node_t *create_record(record_t record)
{
    record_node_t *item = malloc(sizeof(*item));

    if (item == NULL) {
        return NULL;
    }

    item->record = record;
    item->next = NULL;

    return item;
}

/* Insert one record into the tree */
int store_insert(store_t *s, record_t record)
{
    if (s == NULL) {
        return -1;
    }

    int key = temperature_key(
        record.reading.temperature);

    tree_node_t **link = &s->root;

    /* Find the correct tree position */
    while (*link != NULL) {
        if (key < (*link)->key) {
            link = &(*link)->left;
        }
        else if (key > (*link)->key) {
            link = &(*link)->right;
        }
        else {
            /* Same key: add to its list */
            record_node_t *item = create_record(record);

            if (item == NULL) {
                return -1;
            }

            (*link)->last->next = item;
            (*link)->last = item;
            s->count++;

            return 0;
        }
    }

    /* Create a new tree node */
    tree_node_t *node = malloc(sizeof(*node));

    if (node == NULL) {
        return -1;
    }

    record_node_t *item = create_record(record);

    if (item == NULL) {
        free(node);
        return -1;
    }

    node->key = key;
    node->records = item;
    node->last = item;
    node->left = NULL;
    node->right = NULL;

    *link = node;
    s->count++;

    return 0;
}

/* Return number of records */
int store_count(const store_t *s)
{
    if (s == NULL) {
        return 0;
    }

    return s->count;
}

/* Find the first record with a matching key */
const record_t *store_find(
    const store_t *s, float temperature)
{
    if (s == NULL) {
        return NULL;
    }

    int key = temperature_key(temperature);
    tree_node_t *current = s->root;

    while (current != NULL) {
        if (key < current->key) {
            current = current->left;
        }
        else if (key > current->key) {
            current = current->right;
        }
        else {
            return &current->records->record;
        }
    }

    return NULL;
}

/* Visit nodes from smallest to largest */
static void walk_nodes(
    const tree_node_t *node,
    store_callback_t callback,
    void *context)
{
    if (node == NULL) {
        return;
    }

    /* Visit left subtree */
    walk_nodes(node->left, callback, context);

    /* Visit every record with this key */
    const record_node_t *item = node->records;

    while (item != NULL) {
        callback(&item->record, context);
        item = item->next;
    }

    /* Visit right subtree */
    walk_nodes(node->right, callback, context);
}

/* Visit all records in sorted order */
void store_walk(
    const store_t *s,
    store_callback_t callback,
    void *context)
{
    if (s == NULL || callback == NULL) {
        return;
    }

    walk_nodes(s->root, callback, context);
}

/* Calculate height of a tree node */
static int node_height(const tree_node_t *node)
{
    if (node == NULL) {
        return 0;
    }

    int left_height = node_height(node->left);
    int right_height = node_height(node->right);

    if (left_height > right_height) {
        return left_height + 1;
    }

    return right_height + 1;
}

/* Return the height of the tree */
int store_height(const store_t *s)
{
    if (s == NULL) {
        return 0;
    }

    return node_height(s->root);
}

/* Free every node and its records */
static void destroy_nodes(tree_node_t *node)
{
    if (node == NULL) {
        return;
    }

    destroy_nodes(node->left);
    destroy_nodes(node->right);

    record_node_t *item = node->records;

    while (item != NULL) {
        record_node_t *next = item->next;
        free(item);
        item = next;
    }

    free(node);
}

/* Free the whole store */
void store_destroy(store_t *s)
{
    if (s == NULL) {
        return;
    }

    destroy_nodes(s->root);
    free(s);
}
