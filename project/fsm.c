
#include "fsm.h"
#include <stddef.h>

/* Each entry has the next state and action */
#define T(state, action) { state, action }

static const transition_t transitions[STATE_COUNT][EVENT_COUNT] = {

    /* STABLE */
    {
        T(STABLE,   ACT_OFF),
        T(COOLING,  ACT_COOL),
        T(STABLE,   ACT_OFF),
        T(HUMIDIFY, ACT_HUMIDIFY),
        T(STABLE,   ACT_OFF),
        T(ALARM,    ACT_ALARM),
        T(STABLE,   ACT_OFF)
    },

    /* COOLING */
    {
        T(COOLING, ACT_COOL),
        T(COOLING, ACT_COOL),
        T(STABLE,  ACT_OFF),
        T(COOLING, ACT_COOL),
        T(COOLING, ACT_COOL),
        T(ALARM,   ACT_ALARM),
        T(COOLING, ACT_COOL)
    },

    /* HUMIDIFY */
    {
        T(HUMIDIFY, ACT_HUMIDIFY),
        T(COOLING,  ACT_COOL),
        T(HUMIDIFY, ACT_HUMIDIFY),
        T(HUMIDIFY, ACT_HUMIDIFY),
        T(STABLE,   ACT_OFF),
        T(ALARM,    ACT_ALARM),
        T(HUMIDIFY, ACT_HUMIDIFY)
    },

    /* ALARM */
    {
        T(ALARM,  ACT_ALARM),
        T(ALARM,  ACT_ALARM),
        T(ALARM,  ACT_ALARM),
        T(ALARM,  ACT_ALARM),
        T(ALARM,  ACT_ALARM),
        T(ALARM,  ACT_ALARM),
        T(STABLE, ACT_OFF)
    }
};

#undef T

/* Check whether state and event are valid */
static int valid_input(state_t current, event_t event)
{
    return current >= STABLE &&
           current < STATE_COUNT &&
           event >= EV_NONE &&
           event < EVENT_COUNT;
}

/* Return the next state */
state_t fsm_next(state_t current, event_t event)
{
    if (!valid_input(current, event)) {
        return current;
    }

    return transitions[current][event].next_state;
}

/* Return the output action */
action_t fsm_action(state_t current, event_t event)
{
    if (!valid_input(current, event)) {
        return ACT_OFF;
    }

    return transitions[current][event].action;
}

/* Choose an event from sensor readings */
event_t fsm_get_event(state_t current,
                      const reading_t *reading,
                      int ack_pressed)
{
    if (reading == NULL) {
        return EV_NONE;
    }

    float temp = reading->temperature;
    float humidity = reading->humidity;

    /* Alarm has highest priority */
    if (temp > 18.0f || temp < 8.0f) {
        return EV_ALARM;
    }

    /* Alarm needs ACK to clear */
    if (current == ALARM) {
        if (ack_pressed) {
            return EV_ACK;
        }

        return EV_NONE;
    }

    /* Stop cooling at 12.5 C */
    if (current == COOLING && temp <= 12.5f) {
        return EV_COOL_DONE;
    }

    /* Cooling has priority over humidifying */
    if (temp > 14.0f) {
        return EV_TOO_HOT;
    }

    /* Stop humidifying at 68% */
    if (current == HUMIDIFY && humidity >= 68.0f) {
        return EV_HUMID_DONE;
    }

    /* Start humidifying below 60% */
    if (humidity < 60.0f && current != COOLING) {
        return EV_TOO_DRY;
    }

    return EV_NONE;
}

/* Update the state using the transition table */
state_t fsm_update(state_t current,
                   const reading_t *reading,
                   int ack_pressed)
{
    event_t event =
        fsm_get_event(current, reading, ack_pressed);

    return fsm_next(current, event);
}

/* Convert a state to text */
const char *fsm_state_name(state_t state)
{
    switch (state) {
        case STABLE:
            return "STABLE";

        case COOLING:
            return "COOLING";

        case HUMIDIFY:
            return "HUMIDIFY";

        case ALARM:
            return "ALARM";

        default:
            return "UNKNOWN";
    }
}
