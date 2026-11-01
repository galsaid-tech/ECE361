
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "fsm.h"
#include "store.h"
#include "aux.h"
#include "report.h"
#include "bits.h"

/* One change in the scenario file */
typedef struct {
    long long tick;
    float temperature;
    float humidity;
    unsigned int switches;
} scenario_t;

/* Create the status word */
static uint32_t make_status(state_t state)
{
    uint32_t status = 0;

    status = set_field(status, 0, 2, (uint32_t)state);
    status = set_field(status, 2, 1, state == COOLING);
    status = set_field(status, 3, 1, state == HUMIDIFY);
    status = set_field(status, 4, 1, state == ALARM);
    status = set_field(status, 5, 1, state == ALARM);

    return status;
}

/* Choose the three normal LEDs */
static uint32_t make_leds(state_t state)
{
    if (state == COOLING) {
        return 0x1u;
    }

    if (state == HUMIDIFY) {
        return 0x2u;
    }

    if (state == ALARM) {
        return 0x4u;
    }

    return 0u;
}

/* Choose one RGB color for each state */
static uint32_t make_rgb(state_t state)
{
    switch (state) {
        case STABLE:
            return build_rgb_reg(1, 0, 255, 0);

        case COOLING:
            return build_rgb_reg(1, 0, 0, 255);

        case HUMIDIFY:
            return build_rgb_reg(1, 0, 255, 255);

        case ALARM:
            return build_rgb_reg(1, 255, 0, 0);

        default:
            return build_rgb_reg(0, 0, 0, 0);
    }
}

/* Print a UTC timestamp */
static int print_trace(time_t timestamp,
                       state_t state,
                       const reading_t *reading,
                       uint32_t leds,
                       uint32_t status)
{
    char date[32];

    struct tm *utc = gmtime(&timestamp);

    if (utc == NULL) {
        return -1;
    }

    if (strftime(date, sizeof(date),
                 "%Y-%m-%d %H:%M:%S", utc) == 0) {
        return -1;
    }

    printf("%s state=%s temp=%.1f rh=%.1f "
           "leds=0x%X status=0x%08X\n",
           date,
           fsm_state_name(state),
           reading->temperature,
           reading->humidity,
           (unsigned int)leds,
           (unsigned int)status);

    return 0;
}

/* Read the scenario file */
static int load_scenario(const char *filename,
                         time_t *start,
                         scenario_t **events,
                         size_t *event_count)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror(filename);
        return -1;
    }

    char line[256];
    int has_start = 0;
    long long previous_tick = -1;
    scenario_t *items = NULL;
    size_t count = 0;
    size_t capacity = 0;

    while (fgets(line, sizeof(line), file) != NULL) {

        /* Skip comments and blank lines */
        char *p = line;

        while (*p == ' ' || *p == '\t') {
            p++;
        }

        if (*p == '#' || *p == '\n' || *p == '\0') {
            continue;
        }

        if (!has_start) {
            long long start_seconds;

            if (sscanf(p, "start %lld",
                       &start_seconds) != 1 ||
                start_seconds < 0) {
                fprintf(stderr, "Invalid start time.\n");
                goto error;
            }

            *start = (time_t)start_seconds;

            if ((long long)*start != start_seconds) {
                fprintf(stderr,
                        "Start time is out of range.\n");
                goto error;
            }

            has_start = 1;
            continue;
        }

        scenario_t item;
        unsigned int switches;

        if (sscanf(p, "%lld %f %f %x",
                   &item.tick,
                   &item.temperature,
                   &item.humidity,
                   &switches) != 4) {
            fprintf(stderr, "Invalid scenario line.\n");
            goto error;
        }

        if (item.tick <= previous_tick ||
            item.tick < 0 || switches > 1u ||
            !(item.temperature >= -50.0f &&
              item.temperature <= 150.0f) ||
            !(item.humidity >= 0.0f &&
              item.humidity <= 100.0f)) {
            fprintf(stderr, "Invalid scenario values.\n");
            goto error;
        }

        item.switches = switches;
        previous_tick = item.tick;

        if (count == capacity) {
            size_t new_capacity =
                capacity == 0 ? 16 : capacity * 2;

            scenario_t *new_items =
                realloc(items,
                        new_capacity * sizeof(*items));

            if (new_items == NULL) {
                fprintf(stderr, "Out of memory.\n");
                goto error;
            }

            items = new_items;
            capacity = new_capacity;
        }

        items[count++] = item;
    }

    if (ferror(file) || !has_start || count == 0) {
        fprintf(stderr, "Empty or invalid scenario.\n");
        goto error;
    }

    fclose(file);

    *events = items;
    *event_count = count;

    return 0;

error:
    free(items);
    fclose(file);
    return -1;
}

/* Add up ticks from the hash table */
static void add_state_ticks(const aux_entry_t *entry,
                            void *context)
{
    unsigned long long *total = context;

    if (entry != NULL && total != NULL) {
        *total += entry->total_ticks;
    }
}

/* Main program */
int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3) {
        fprintf(stderr,
                "Usage: %s scenario.txt [--report]\n",
                argv[0]);
        return 1;
    }

    int report_only = 0;

    if (argc == 3) {
        if (strcmp(argv[2], "--report") != 0) {
            fprintf(stderr, "Unknown option.\n");
            return 1;
        }

        report_only = 1;
    }

    time_t start = 0;
    scenario_t *events = NULL;
    size_t event_count = 0;

    if (load_scenario(argv[1], &start,
                      &events, &event_count) != 0) {
        return 1;
    }

    int rc = 0;

    uint32_t *base = iom361_initialize(1, 3, &rc);

    if (base == NULL || rc != 0) {
        fprintf(stderr, "Initialization failed.\n");
        free(events);
        return 1;
    }

    /* Hide hardware display messages */
    iom361_set_display(false);

    store_t *store = store_create();
    aux_t *table = aux_create();

    if (store == NULL || table == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        store_destroy(store);
        aux_destroy(table);
        free(events);
        return 1;
    }

    state_t state = STABLE;

    float temperature = 23.5f;
    float humidity = 75.0f;
    unsigned int switches = 0;
    unsigned int previous_switches = 0;

    size_t next_event = 0;
    int failed = 0;

    /* Run once for each tick, including the last */
    long long last_tick = events[event_count - 1].tick;

    for (long long tick = 0; tick <= last_tick; tick++) {

        /* Apply scenario changes at this tick */
        if (next_event < event_count &&
            events[next_event].tick == tick) {

            temperature = events[next_event].temperature;
            humidity = events[next_event].humidity;
            switches = events[next_event].switches;

            next_event++;
        }

        /* Set the simulated environment */
        _iom361_set_sensor1(temperature, humidity);
        _iom361_set_switches(switches);

        time_t timestamp = start + (time_t)tick;

        /* Read temperature and humidity */
        reading_t reading = {0};

        if (sensor_read(base, timestamp, &reading) != 0) {
            fprintf(stderr, "Sensor read failed.\n");
            failed = 1;
            break;
        }

        /* Read the switch */
        uint32_t switch_value =
            iom361_readReg(base, SWITCHES_REG, &rc);

        if (rc != 0) {
            fprintf(stderr, "Switch read failed.\n");
            failed = 1;
            break;
        }

        /* ACK counts only on a 0-to-1 change */
        int ack_pressed =
            (switch_value & 1u) != 0 &&
            (previous_switches & 1u) == 0;

        previous_switches = switch_value;

        /* Update the state machine */
        state = fsm_update(state, &reading,
                           ack_pressed);

        /* Create LED and status values */
        uint32_t leds = make_leds(state);
        uint32_t rgb = make_rgb(state);
        uint32_t status = make_status(state);

        /* Write the LEDs */
        iom361_writeReg(base, LEDS_REG, leds, &rc);

        if (rc != 0) {
            failed = 1;
            break;
        }

        iom361_writeReg(base, RGB_LED_REG, rgb, &rc);

        if (rc != 0) {
            failed = 1;
            break;
        }

        /* Save the reading in the BST */
        record_t record = {0};

        record.reading = reading;
        record.state = state;
        record.status = status;

        if (store_insert(store, record) != 0) {
            fprintf(stderr, "Store insert failed.\n");
            failed = 1;
            break;
        }

        /* Count one second in this state */
        if (aux_update(table, state, 1) != 0) {
            fprintf(stderr, "Hash table update failed.\n");
            failed = 1;
            break;
        }

        /* Print one trace line per tick */
        if (!report_only) {
            if (print_trace(timestamp, state,
                            &reading, leds,
                            status) != 0) {
                failed = 1;
                break;
            }
        }
    }

    /* Check that all state ticks were counted */
    if (!failed) {
        unsigned long long total_ticks = 0;

        aux_walk(table, add_state_ticks, &total_ticks);

        unsigned long long expected_ticks =
            (unsigned long long)last_tick + 1ULL;

        if (total_ticks != expected_ticks) {
            fprintf(stderr,
                    "Hash table total ticks mismatch.\n");
            failed = 1;
        }
    }

    /* Print the report if requested */
    if (!failed && report_only) {
        report_result_t result;

        if (report_generate(store, &result) != 0) {
            failed = 1;
        }
        else {
            report_print(&result, start);
        }
    }

    /* Free allocated memory */
    store_destroy(store);
    aux_destroy(table);
    free(events);

    return failed ? 1 : 0;
}
