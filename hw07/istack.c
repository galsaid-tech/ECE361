#include "istack.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Only this file knows the contents of the opaque stack. */
struct istack {
    int *data;
    size_t count;
    size_t capacity;
};

istack_t *istack_create(void)
{
    istack_t *s = malloc(sizeof(*s));
    if (s == NULL) {
        return NULL;
    }

    s->data = malloc(ISTACK_START * sizeof(*s->data));
    if (s->data == NULL) {
        free(s);
        return NULL;
    }

    s->count = 0;
    s->capacity = ISTACK_START;
    return s;
}

void istack_destroy(istack_t *s)
{
    if (s == NULL) {
        return;
    }
    free(s->data);
    free(s);
}

int istack_push(istack_t *s, int v)
{
    if (s == NULL) {
        return -1;
    }
    if (s->count == s->capacity) {
        if (s->capacity > SIZE_MAX / 2 ||
            2 * s->capacity > SIZE_MAX / sizeof(*s->data)) {
            return -1;
        }
        size_t new_capacity = s->capacity * 2;
        /* A temporary pointer means failure does not lose the old array. */
        int *new_data = realloc(s->data, new_capacity * sizeof(*s->data));
        if (new_data == NULL) {
            return -1;
        }
        s->data = new_data;
        s->capacity = new_capacity;
    }
    s->data[s->count] = v;
    s->count++;
    return 0;
}

int istack_pop(istack_t *s, int *v)
{
    if (s == NULL || s->count == 0) {
        return -1;
    }
    s->count--;
    if (v != NULL) {
        *v = s->data[s->count];
    }
    return 0;
}

int istack_peek(const istack_t *s, int *v)
{
    if (s == NULL || s->count == 0) {
        return -1;
    }
    if (v != NULL) {
        *v = s->data[s->count - 1];
    }
    return 0;
}

size_t istack_count(const istack_t *s)
{
    return s == NULL ? 0 : s->count;
}

bool istack_is_empty(const istack_t *s)
{
    return s == NULL || s->count == 0;
}

size_t istack_capacity(const istack_t *s)
{
    return s == NULL ? 0 : s->capacity;
}

int *istack_to_array(const istack_t *s, size_t *n)
{
    if (n != NULL) {
        *n = 0;
    }
    if (s == NULL || s->count == 0 ||
        s->count > SIZE_MAX / sizeof(*s->data)) {
        return NULL;
    }
    int *copy = malloc(s->count * sizeof(*copy));
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, s->data, s->count * sizeof(*copy));
    if (n != NULL) {
        *n = s->count;
    }
    return copy;
}
