#include "reading.h"

/*
 * Reads sensor readings from a file.
 * Ignores empty lines and comments.
 * Counts lines with invalid data as skipped.
 */
int reading_read(FILE *input, int ticks[],
                 float temps[], float hums[],
                 int *skipped)
{
    char line[128];
    int count = 0;

    *skipped = 0;

    while (fgets(line, sizeof line, input) != NULL) {
        int i = 0;

        while (line[i] == ' ' || line[i] == '\t')
            i++;

        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#')
            continue;

        if (count == MAX_READINGS) {
            fprintf(stderr,
                    "warning: more than %d readings, the rest are ignored\n",
                    MAX_READINGS);
            break;
        }

        if (sscanf(line, "%d %f %f",
                   &ticks[count], &temps[count], &hums[count]) == 3)
            count++;
        else
            (*skipped)++;
    }

    return count;
}