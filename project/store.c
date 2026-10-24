
#include "store.h"
#include <stdlib.h>

/* Maximum records for now */
#define STORE_MAX 100

/* Hidden store structure */
struct store {
    record_t records[STORE_MAX];
    int count;
};

/* Convert temperature into a 0.5 C key */
static int temperature_key(float temperature) {
    int key = (int)(temperature * 2.0f);
    return key;
}

/* Compare two records */
int record_compare(const record_t *a, const record_t *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }

    int key_a = temperature_key(a->reading.temperature);
    int key_b = temperature_key(b->reading.temperature);

    if (key_a < key_b) {
        return -1;
    }

    if (key_a > key_b) {
        return 1;
    }

    return 0;
}

/* Create an empty store */
store_t *store_create(void) {
    store_t *s = malloc(sizeof(store_t));

    if (s == NULL) {
        return NULL;
    }

    s->count = 0;
    return s;
}

/* Add a record in temperature key order */
int store_insert(store_t *s, record_t record) {
    if (s == NULL || s->count >= STORE_MAX) {
        return -1;
    }

    int pos = 0;

    while (pos < s->count &&
           record_compare(&s->records[pos], &record) <= 0) {
        pos++;
    }

    /* Move records to make space */
    for (int i = s->count; i > pos; i--) {
        s->records[i] = s->records[i - 1];
    }

    s->records[pos] = record;
    s->count++;

    return 0;
}

/* Return the number of stored records */
int store_count(const store_t *s) {
    if (s == NULL) {
        return 0;
    }

    return s->count;
}

/* Find a record using its temperature key */
const record_t *store_find(const store_t *s, float temperature) {
    if (s == NULL) {
        return NULL;
    }

    int key = temperature_key(temperature);

    for (int i = 0; i < s->count; i++) {
        int current_key =
            temperature_key(s->records[i].reading.temperature);

        if (current_key == key) {
            return &s->records[i];
        }
    }

    return NULL;
}

/* Visit every stored record in order */
void store_walk(const store_t *s,
                store_callback_t callback,
                void *context) {
    if (s == NULL || callback == NULL) {
        return;
    }

    for (int i = 0; i < s->count; i++) {
        callback(&s->records[i], context);
    }
}

/* Free the store */
void store_destroy(store_t *s) {
    free(s);
}
