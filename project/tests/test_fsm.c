
#include <stdio.h>
#include <assert.h>
#include "fsm.h"

static void test_actions(void)
{
    assert(fsm_action(STABLE, EV_TOO_HOT) == ACT_COOL);
    assert(fsm_action(STABLE, EV_TOO_DRY) == ACT_HUMIDIFY);
    assert(fsm_action(COOLING, EV_COOL_DONE) == ACT_OFF);
    assert(fsm_action(HUMIDIFY, EV_TOO_HOT) == ACT_COOL);
    assert(fsm_action(STABLE, EV_ALARM) == ACT_ALARM);
    assert(fsm_action(ALARM, EV_ACK) == ACT_OFF);

    printf("FSM actions - PASSED\n");
}

int main(void)
{
    reading_t r = {0};
    state_t state = STABLE;

    /* Test 1: Start cooling */
    r.temperature = 15.0f;
    r.humidity = 65.0f;
    state = fsm_update(state, &r, 0);
    assert(state == COOLING);
    printf("Test 1: Cooling starts - PASSED\n");

    /* Test 2: Cooling continues */
    r.temperature = 13.0f;
    state = fsm_update(state, &r, 0);
    assert(state == COOLING);
    printf("Test 2: Cooling continues - PASSED\n");

    /* Test 3: Cooling stops */
    r.temperature = 12.5f;
    state = fsm_update(state, &r, 0);
    assert(state == STABLE);
    printf("Test 3: Cooling stops - PASSED\n");

    /* Test 4: Start humidifying */
    r.temperature = 13.0f;
    r.humidity = 58.0f;
    state = fsm_update(state, &r, 0);
    assert(state == HUMIDIFY);
    printf("Test 4: Humidifying starts - PASSED\n");

    /* Test 5: Humidifying continues */
    r.humidity = 65.0f;
    state = fsm_update(state, &r, 0);
    assert(state == HUMIDIFY);
    printf("Test 5: Humidifying continues - PASSED\n");

    /* Test 6: Humidifying stops */
    r.humidity = 68.0f;
    state = fsm_update(state, &r, 0);
    assert(state == STABLE);
    printf("Test 6: Humidifying stops - PASSED\n");

    /* Test 7: Cooling has priority */
    r.temperature = 15.0f;
    r.humidity = 58.0f;
    state = fsm_update(state, &r, 0);
    assert(state == COOLING);
    printf("Test 7: Cooling priority - PASSED\n");

    /* Test 8: High temperature alarm */
    r.temperature = 19.0f;
    state = fsm_update(state, &r, 0);
    assert(state == ALARM);
    printf("Test 8: Alarm starts - PASSED\n");

    /* Test 9: ACK cannot clear unsafe alarm */
    state = fsm_update(state, &r, 1);
    assert(state == ALARM);
    printf("Test 9: Unsafe ACK - PASSED\n");

    /* Test 10: Alarm stays until ACK */
    r.temperature = 13.0f;
    state = fsm_update(state, &r, 0);
    assert(state == ALARM);
    printf("Test 10: Alarm stays active - PASSED\n");

    /* Test 11: Safe ACK clears alarm */
    state = fsm_update(state, &r, 1);
    assert(state == STABLE);
    printf("Test 11: Safe ACK - PASSED\n");

    /* Test 12: Low temperature alarm */
    r.temperature = 7.0f;
    state = fsm_update(state, &r, 0);
    assert(state == ALARM);
    printf("Test 12: Low temperature alarm - PASSED\n");

    printf("All 12 FSM tests passed!\n");
    test_actions();
    return 0;
}
