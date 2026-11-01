
#ifndef FSM_H
#define FSM_H

#include "sensor.h"

/* The four device states */
typedef enum {
    STABLE,
    COOLING,
    HUMIDIFY,
    ALARM,
    STATE_COUNT
} state_t;

/* Events that change the state */
typedef enum {
    EV_NONE,
    EV_TOO_HOT,
    EV_COOL_DONE,
    EV_TOO_DRY,
    EV_HUMID_DONE,
    EV_ALARM,
    EV_ACK,
    EVENT_COUNT
} event_t;

/* Actions for the device outputs */
typedef enum {
    ACT_OFF,
    ACT_COOL,
    ACT_HUMIDIFY,
    ACT_ALARM
} action_t;

/* One entry in the transition table */
typedef struct {
    state_t next_state;
    action_t action;
} transition_t;

/* Get next state from the table */
state_t fsm_next(state_t current, event_t event);

/* Get the action from the table */
action_t fsm_action(state_t current, event_t event);

/* Choose an event using sensor values */
event_t fsm_get_event(state_t current,
                      const reading_t *reading,
                      int ack_pressed);

/* Update the device state */
state_t fsm_update(state_t current,
                   const reading_t *reading,
                   int ack_pressed);

/* Get the state name */
const char *fsm_state_name(state_t state);

#endif
