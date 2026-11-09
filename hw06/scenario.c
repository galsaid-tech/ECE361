
#include "scenario.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int scenario_load(const char *filename, scenario_t *scenario)
{
    if (filename == NULL || scenario == NULL) {
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 1;
    }

    char text[256];
    int line_number = 0;
    int has_start = 0;
    int count = 0;
    time_t start = 0;
    scenario_line_t lines[MAX_SCENARIO_LINES];

    while (fgets(text, sizeof(text), file) != NULL) {
        line_number++;

        // Reject lines that are too long
        if (strchr(text, '\n') == NULL &&
            !feof(file)) {
            fclose(file);
            return line_number;
        }

        // Remove comments
        char *comment = strchr(text, '#');
        if (comment != NULL) {
            *comment = '\0';
        }

        // Skip spaces and blank lines
        char *p = text;
        while (isspace((unsigned char)*p)) {
            p++;
        }

        if (*p == '\0') {
            continue;
        }

        // Read the starting timestamp
        if (!has_start) {
            long long timestamp;
            int used = 0;

            if (sscanf(p, "start %lld %n",
                       &timestamp, &used) != 1 ||
                used == 0 || p[used] != '\0' ||
                (long long)(time_t)timestamp != timestamp) {
                fclose(file);
                return line_number;
            }

            start = (time_t)timestamp;
            has_start = 1;
            continue;
        }

        // Read one scenario entry
        scenario_line_t entry;
        unsigned int switches;
        int used = 0;

        if (sscanf(p, "%d %f %f %x %n",
                   &entry.tick,
                   &entry.temp_c,
                   &entry.rh_pct,
                   &switches,
                   &used) != 4 ||
            used == 0 || p[used] != '\0') {
            fclose(file);
            return line_number;
        }

        // Check the tick order
        if (entry.tick < 0 ||
            (count > 0 &&
             entry.tick <= lines[count - 1].tick)) {
            fclose(file);
            return line_number;
        }

        // Check the maximum number of entries
        if (count >= MAX_SCENARIO_LINES) {
            fclose(file);
            return line_number;
        }

        entry.switches = (uint32_t)switches;
        lines[count] = entry;
        count++;
    }

    fclose(file);

    if (!has_start || count == 0) {
        return line_number > 0 ? line_number : 1;
    }

    // Save only after the file is valid
    scenario->start = start;
    scenario->count = count;

    for (int i = 0; i < count; i++) {
        scenario->lines[i] = lines[i];
    }

    return 0;
}

const scenario_line_t *scenario_at(
    const scenario_t *scenario, int tick)
{
    if (scenario == NULL ||
        scenario->count == 0 ||
        tick < scenario->lines[0].tick) {
        return NULL;
    }

    // Find the latest entry for this tick
    const scenario_line_t *result =
        &scenario->lines[0];

    for (int i = 1; i < scenario->count; i++) {
        if (scenario->lines[i].tick > tick) {
            break;
        }

        result = &scenario->lines[i];
    }

    return result;
}
