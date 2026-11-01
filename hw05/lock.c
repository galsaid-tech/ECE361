
#include "lock.h"
#include <ctype.h>

/* Table of state changes and actions. */
const transition_t lock_table[N_STATES][N_EVENTS] = {
    [LOCKED] = {
        [E_RIGHT]   = {GOT1, A_NONE},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {LOCKED, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },
    [GOT1] = {
        [E_RIGHT]   = {GOT2, A_NONE},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {GOT1, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },
    [GOT2] = {
        [E_RIGHT]   = {OPEN, A_UNLOCK},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {GOT2, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },
    [OPEN] = {
        [E_RIGHT]   = {OPEN, A_NONE},
        [E_WRONG]   = {OPEN, A_NONE},
        [E_CLOSE]   = {LOCKED, A_LOCK},
        [E_TIMEOUT] = {LOCKED, A_LOCK}
    }
};

/* Uses the table to find the next state and action. */
state_t lock_step(state_t s, event_t e, action_t *action)
{
    transition_t t = lock_table[s][e];
    *action = t.action;
    return t.next;
}

/* Converts input characters into events. */
int lock_event(state_t s, char c)
{
    if (c == 'c')
        return E_CLOSE;

    if (c == 't')
        return E_TIMEOUT;

    if (c >= '0' && c <= '9') {
        if ((s == LOCKED && c == '3') ||
            (s == GOT1 && c == '1') ||
            (s == GOT2 && c == '4'))
            return E_RIGHT;

        return E_WRONG;
    }

    return -1;
}

/* Returns a state name. */
const char *state_name(state_t s)
{
    static const char *names[] = {
        "LOCKED", "GOT1", "GOT2", "OPEN"
    };

    if (s < 0 || s >= N_STATES)
        return "?";

    return names[s];
}

/* Returns an event name. */
const char *event_name(event_t e)
{
    static const char *names[] = {
        "RIGHT", "WRONG", "CLOSE", "TIMEOUT"
    };

    if (e < 0 || e >= N_EVENTS)
        return "?";

    return names[e];
}

/* Returns an action name. */
const char *action_name(action_t a)
{
    static const char *names[] = {
        "UNDEFINED", "NONE", "UNLOCK", "LOCK", "BEEP"
    };

    if (a < 0 || a >= N_ACTIONS)
        return "?";

    return names[a];
}
