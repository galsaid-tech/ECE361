
#include <stdio.h>
#include <string.h>
#include "tally.h"

int main(void) {
    tally_t *t = tally_create();

    if (t == NULL) {
        return 1;
    }

    char line[256];
    int skipped = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);

        /* Remove the newline if it exists */
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        /* Handle a line longer than the buffer */
        else if (len == sizeof(line) - 1) {
            int ch = getchar();

            if (ch != '\n' && ch != EOF) {
                while ((ch = getchar()) != '\n' && ch != EOF) {
                    /* Skip the rest of the line */
                }

                if (line[0] != '#') {
                    skipped++;
                }
                continue;
            }
        }

        /* Skip blank lines and comments */
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        /* Add the name to the tally */
        if (tally_add(t, line) == -1) {
            skipped++;
        }
    }

    tally_print(t, stdout);

    printf("names: %d, lines skipped: %d\n",
           tally_size(t), skipped);

    tally_destroy(t);

    return 0;
}
