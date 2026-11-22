
#include "bst.h"
#include <stdlib.h>

/* A node in the binary search tree */
struct node {
    int data;
    struct node *leftChild;
    struct node *rightChild;
};

/* The tree stores its own root and size */
struct bst {
    struct node *root;
    size_t size;
};

/* Create an empty tree */
bst_t *bst_create(void)
{
    bst_t *t = malloc(sizeof(bst_t));

    if (t == NULL) {
        return NULL;
    }

    t->root = NULL;
    t->size = 0;

    return t;
}

/* Free all nodes */
static void free_nodes(struct node *current)
{
    if (current == NULL) {
        return;
    }

    free_nodes(current->leftChild);
    free_nodes(current->rightChild);
    free(current);
}

/* Destroy the tree */
void bst_destroy(bst_t *t)
{
    if (t == NULL) {
        return;
    }

    free_nodes(t->root);
    free(t);
}

/* Based on the original insert() function */
int bst_insert(bst_t *t, int key)
{
    if (t == NULL) {
        return -1;
    }

    struct node *tempNode = malloc(sizeof(struct node));

    if (tempNode == NULL) {
        return -1;
    }

    tempNode->data = key;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    if (t->root == NULL) {
        t->root = tempNode;
    } else {
        struct node *current = t->root;
        struct node *parent = NULL;

        while (1) {
            parent = current;

            if (key < parent->data) {
                current = current->leftChild;

                if (current == NULL) {
                    parent->leftChild = tempNode;
                    break;
                }
            } else {
                current = current->rightChild;

                if (current == NULL) {
                    parent->rightChild = tempNode;
                    break;
                }
            }
        }
    }

    t->size++;
    return 0;
}

/* Based on the original search() function.
   This version keeps the original bug for testing. */

/* Based on the original search() function.
   Fixed the NULL pointer crash. */
bool bst_contains(const bst_t *t, int key, size_t *visits)
{
    if (visits != NULL) {
        *visits = 0;
    }

    if (t == NULL) {
        return false;
    }

    const struct node *current = t->root;

    while (current != NULL) {
        if (visits != NULL) {
            (*visits)++;
        }

        if (current->data == key) {
            return true;
        }

        if (key < current->data) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }

    return false;
}


/* Return the number of keys */
size_t bst_size(const bst_t *t)
{
    if (t == NULL) {
        return 0;
    }

    return t->size;
}

/* Find the height of a node */
static size_t node_height(const struct node *current)
{
    if (current == NULL) {
        return 0;
    }

    size_t left = node_height(current->leftChild);
    size_t right = node_height(current->rightChild);

    return 1 + (left > right ? left : right);
}

/* Return tree height */
size_t bst_height(const bst_t *t)
{
    if (t == NULL) {
        return 0;
    }

    return node_height(t->root);
}
