
#include <stdio.h>
#include "check.h"

/* Check whether a state number is valid. */
static int valid_state(state_t s)
{
    return s >= LOCKED && s < N_STATES;
}

/* Check the table and return the number of problems. */
int fsm_check(
    const transition_t table[N_STATES][N_EVENTS],
    state_t start,
    FILE *out
)
{
    int problems = 0;
    int visited[N_STATES] = {0};
    state_t queue[N_STATES];
    int front = 0;
    int back = 0;

    /* Check all table entries. */
    for (int s = 0; s < N_STATES; s++) {
        int has_exit = 0;

        for (int e = 0; e < N_EVENTS; e++) {
            transition_t t = table[s][e];

            /* Find missing entries. */
            if (t.action == A_UNDEFINED) {
                fprintf(out, "%s %s: undefined entry\n",
                        state_name((state_t)s),
                        event_name((event_t)e));
                problems++;
            }

            /* Find invalid next states. */
            if (!valid_state(t.next)) {
                fprintf(out, "%s %s: invalid next state\n",
                        state_name((state_t)s),
                        event_name((event_t)e));
                problems++;
            } else if (t.action != A_UNDEFINED &&
                       t.next != (state_t)s) {
                has_exit = 1;
            }
        }

        /* Find states with no way out. */
        if (!has_exit) {
            fprintf(out, "%s: no way out\n",
                    state_name((state_t)s));
            problems++;
        }
    }

    /* Start a breadth-first search. */
    if (!valid_state(start)) {
        fprintf(out, "invalid start state\n");
        problems++;
        return problems;
    }

    visited[start] = 1;
    queue[back++] = start;

    while (front < back) {
        state_t current = queue[front++];

        for (int e = 0; e < N_EVENTS; e++) {
            transition_t t = table[current][e];

            if (t.action == A_UNDEFINED ||
                !valid_state(t.next)) {
                continue;
            }

            if (!visited[t.next]) {
                visited[t.next] = 1;
                queue[back++] = t.next;
            }
        }
    }

    /* Find states that cannot be reached. */
    for (int s = 0; s < N_STATES; s++) {
        if (!visited[s]) {
            fprintf(out, "%s: unreachable\n",
                    state_name((state_t)s));
            problems++;
        }
    }

    return problems;
}
