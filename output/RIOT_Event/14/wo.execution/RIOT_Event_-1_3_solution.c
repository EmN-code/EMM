#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/callback.h"
#include "MQTTClient.h"
#include "time_units.h"

static char event_thread_stack[THREAD_STACKSIZE_MAIN];

void *event_thread(void *arg) {
    (void)arg;
    evtimer_t evtimer;
    msg_t msg;
    int period = 1 * US_PER_SEC; // Set period to 1 second

    evtimer_init_msg(&evtimer);
    evtimer_periodic_start(&evtimer, period, &msg, EVENT_PRIO_LOWEST);

    while (1) {
        evtimer_periodic_trigger(&evtimer);
        thread_yield();
    }

    return NULL;
}

int main(void) {
    thread_create(event_thread_stack, sizeof(event_thread_stack), 
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
                  event_thread, NULL, "event_thread");
    
    // Main loop
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}