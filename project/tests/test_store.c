
#include <stdio.h>
#include "store.h"

/* Print each record */
void print_record(const record_t *record, void *context) {
    (void)context;

    printf("Temperature: %.1f\n",
           record->reading.temperature);
}

int main(void) {
    record_t a = {0};
    record_t b = {0};
    record_t c = {0};

    a.reading.temperature = 13.0f;
    b.reading.temperature = 13.2f;
    c.reading.temperature = 14.0f;

    printf("Same key: %d\n", record_compare(&a, &b));
    printf("Smaller key: %d\n", record_compare(&a, &c));
    printf("Larger key: %d\n", record_compare(&c, &a));

    store_t *s = store_create();

    if (s == NULL) {
        return 1;
    }

    store_insert(s, c);
    store_insert(s, a);
    store_insert(s, b);

    printf("Records stored: %d\n", store_count(s));


/* Test finding a record */
const record_t *found = store_find(s, 13.0f);

if (found != NULL) {
    printf("Found temperature: %.1f\n",
           found->reading.temperature);
} else {
    printf("Record not found\n");
}

/* Test sorted order */
printf("Stored records in order:\n");
store_walk(s, print_record, NULL);

   
/* Test temperature boundaries */
record_t d = {0};
record_t e = {0};
record_t f = {0};

d.reading.temperature = 13.4f;
e.reading.temperature = 13.5f;
f.reading.temperature = -0.2f;

printf("Same step: %d\n", record_compare(&a, &d));
printf("Next step: %d\n", record_compare(&a, &e));
printf("Negative vs zero: %d\n", record_compare(&f, &a));


    store_destroy(s);

    return 0;
}
