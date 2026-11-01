
#include <stdio.h>
#include "lock.h"

int main(void)
{
    state_t state = LOCKED;
    action_t action;
    int ch;

    /* Read one character at a time. */
    while ((ch = getchar()) != EOF) {

        /* Convert the character into an event. */
        int event = lock_event(state, (char)ch);

        /* Skip characters that are not events. */
        if (event == -1)
            continue;

        /* Find the next state using the table. */
        state_t next = lock_step(state, (event_t)event, &action);

        /* Print the result of the event. */
        printf("%s %c %s -> %s %s\n",
               state_name(state),
               (char)ch,
               event_name((event_t)event),
               state_name(next),
               action_name(action));

        /* Update the current state. */
        state = next;
    }

    /* Print the final state. */
    printf("final: %s\n", state_name(state));

    return 0;
}
