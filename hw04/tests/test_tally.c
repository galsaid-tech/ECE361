
#include <stdio.h>
#include <string.h>
#include "tally.h"

static int passed = 0;
static int failed = 0;

static void check(const char *name, int condition) {
    if (condition) {
        printf("PASS: %s\n", name);
        passed++;
    } else {
        printf("FAIL: %s\n", name);
        failed++;
    }
}

int main(void) {
    tally_t *t = tally_create();

    check("Create tally", t != NULL);

    if (t == NULL) {
        return 1;
    }

    check("Empty tally", tally_size(t) == 0);

    check("Add first name", tally_add(t, "apple") == 1);
    check("Count up", tally_add(t, "apple") == 2);
    check("Count existing name", tally_count(t, "apple") == 2);
    check("Name never added", tally_count(t, "orange") == 0);

    check("Empty name", tally_add(t, "") == -1);
    check("Exactly 15 characters",
          tally_add(t, "abcdefghijklmno") == 1);
    check("16 characters",
          tally_add(t, "abcdefghijklmnop") == -1);

    check("NULL tally add", tally_add(NULL, "apple") == -1);
    check("NULL name add", tally_add(t, NULL) == -1);
    check("NULL tally count", tally_count(NULL, "apple") == 0);
    check("NULL name count", tally_count(t, NULL) == 0);
    check("NULL tally size", tally_size(NULL) == 0);

    tally_destroy(NULL);
    check("Destroy NULL", 1);

    tally_destroy(t);

    tally_t *a = tally_create();
    tally_t *b = tally_create();
    tally_t *c = tally_create();
    tally_t *d = tally_create();
    tally_t *extra = tally_create();

    check("Four tallies", a && b && c && d);
    check("Fifth tally refused", extra == NULL);

    if (a && b) {
        tally_add(a, "pear");
        check("Independent tallies",
              tally_count(b, "pear") == 0);
    }

    tally_destroy(a);
    tally_t *again = tally_create();
    check("Create after destroy", again != NULL);

    tally_destroy(again);
    tally_destroy(b);
    tally_destroy(c);
    tally_destroy(d);

    t = tally_create();

    if (t == NULL) {
        return 1;
    }

    char name[16];

    for (int i = 0; i < TALLY_NAMES; i++) {
        snprintf(name, sizeof(name), "name%02d", i);
        tally_add(t, name);
    }

    check("Full tally size", tally_size(t) == TALLY_NAMES);
    check("New name refused when full",
          tally_add(t, "extra") == -1);
    check("Existing name still counts",
          tally_add(t, "name00") == 2);

    tally_destroy(t);

    t = tally_create();

    tally_add(t, "zebra");
    tally_add(t, "apple");
    tally_add(t, "middle");

    FILE *file = tmpfile();

    if (file != NULL) {
        tally_print(t, file);
        rewind(file);

        char output[256] = "";
        size_t n = fread(output, 1, sizeof(output) - 1, file);
        output[n] = '\0';

        check("Alphabetical print order",
              strcmp(output,
                     "apple 1\nmiddle 1\nzebra 1\n") == 0);

        fclose(file);
    } else {
        check("Alphabetical print order", 0);
    }

    tally_destroy(t);

    printf("\nSummary: %d passed, %d failed\n",
           passed, failed);

    return failed == 0 ? 0 : 1;
}
