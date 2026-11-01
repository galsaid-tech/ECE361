
#include <stdio.h>
#include <string.h>
#include "lock.h"

static int passed = 0;
static int failed = 0;

/* Print PASS or FAIL for each test. */
static void check(int condition, const char *name)
{
    if (condition) {
        printf("PASS: %s\n", name);
        passed++;
    } else {
        printf("FAIL: %s\n", name);
        failed++;
    }
}

/* Test a complete sequence of input characters. */
static state_t run_code(const char *code)
{
    state_t state = LOCKED;

    for (int i = 0; code[i] != '\0'; i++) {
        int event = lock_event(state, code[i]);

        if (event == -1)
            continue;

        action_t action;
        state = lock_step(state, (event_t)event, &action);
    }

    return state;
}

int main(void)
{
    /* Expected results for all 16 transitions. */
    const transition_t expected[N_STATES][N_EVENTS] = {
        {
            {GOT1, A_NONE},
            {LOCKED, A_BEEP},
            {LOCKED, A_NONE},
            {LOCKED, A_NONE}
        },
        {
            {GOT2, A_NONE},
            {LOCKED, A_BEEP},
            {GOT1, A_NONE},
            {LOCKED, A_NONE}
        },
        {
            {OPEN, A_UNLOCK},
            {LOCKED, A_BEEP},
            {GOT2, A_NONE},
            {LOCKED, A_NONE}
        },
        {
            {OPEN, A_NONE},
            {OPEN, A_NONE},
            {LOCKED, A_LOCK},
            {LOCKED, A_LOCK}
        }
    };

    /* Check every table entry and lock_step. */
    for (int s = 0; s < N_STATES; s++) {
        for (int e = 0; e < N_EVENTS; e++) {
            action_t action = A_UNDEFINED;
            state_t next = lock_step(
                (state_t)s, (event_t)e, &action
            );

            char name[80];
            snprintf(name, sizeof(name),
                     "transition %s %s",
                     state_name((state_t)s),
                     event_name((event_t)e));

            check(
                lock_table[s][e].next == expected[s][e].next &&
                lock_table[s][e].action == expected[s][e].action &&
                next == expected[s][e].next &&
                action == expected[s][e].action,
                name
            );
        }
    }

    /* Test right and wrong digits. */
    check(lock_event(LOCKED, '3') == E_RIGHT,
          "LOCKED correct digit");
    check(lock_event(LOCKED, '1') == E_WRONG,
          "LOCKED wrong digit");

    check(lock_event(GOT1, '1') == E_RIGHT,
          "GOT1 correct digit");
    check(lock_event(GOT1, '3') == E_WRONG,
          "GOT1 wrong digit");

    check(lock_event(GOT2, '4') == E_RIGHT,
          "GOT2 correct digit");
    check(lock_event(GOT2, '1') == E_WRONG,
          "GOT2 wrong digit");

    check(lock_event(OPEN, '3') == E_WRONG,
          "OPEN digit 3");
    check(lock_event(OPEN, '4') == E_WRONG,
          "OPEN digit 4");

    /* Test special characters in every state. */
    for (int s = 0; s < N_STATES; s++) {
        char name[80];

        snprintf(name, sizeof(name),
                 "%s close", state_name((state_t)s));
        check(lock_event((state_t)s, 'c') == E_CLOSE, name);

        snprintf(name, sizeof(name),
                 "%s timeout", state_name((state_t)s));
        check(lock_event((state_t)s, 't') == E_TIMEOUT, name);

        snprintf(name, sizeof(name),
                 "%s invalid character", state_name((state_t)s));
        check(lock_event((state_t)s, 'x') == -1, name);
    }

    /* Test the required sequences. */
    check(run_code("314") == OPEN,
          "314 opens");

    check(run_code("3314") == LOCKED,
          "3314 does not open");

    check(run_code("31t4") == LOCKED,
          "31t4 does not open");

    check(run_code("314c") == LOCKED,
          "314c locks again");

    /* Test the name functions. */
    check(strcmp(state_name(LOCKED), "LOCKED") == 0,
          "state name");
    check(strcmp(event_name(E_RIGHT), "RIGHT") == 0,
          "event name");
    check(strcmp(action_name(A_UNLOCK), "UNLOCK") == 0,
          "action name");

    check(strcmp(state_name(N_STATES), "?") == 0,
          "invalid state name");
    check(strcmp(event_name(N_EVENTS), "?") == 0,
          "invalid event name");
    check(strcmp(action_name(N_ACTIONS), "?") == 0,
          "invalid action name");

    /* Print the final summary. */
    printf("\nSummary: %d passed, %d failed\n",
           passed, failed);

    return failed == 0 ? 0 : 1;
}
