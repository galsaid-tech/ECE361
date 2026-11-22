
#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    bst_t *t = bst_create();

    if (t == NULL) {
        fprintf(stderr, "Memory error\n");
        return 1;
    }

    int key;
    int result;

    /* Read integers until the end of input */
    while ((result = scanf("%d", &key)) == 1) {
        if (bst_insert(t, key) != 0) {
            fprintf(stderr, "Memory error\n");
            bst_destroy(t);
            return 1;
        }
    }

    /* Check for invalid input */
    if (result == 0) {
        printf("not a number after %zu values\n",
               bst_size(t));
        bst_destroy(t);
        return 1;
    }

    /* Check if the tree is empty */
    if (bst_size(t) == 0) {
        printf("empty tree\n");
    } else {
        printf("n %zu, height %zu\n",
               bst_size(t), bst_height(t));
    }

    bst_destroy(t);
    return 0;
}
