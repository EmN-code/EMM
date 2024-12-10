
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_msg_t evtimer;
    evtimer_msg_init(&evtimer);

    // Set up the periodic event
    evtimer_msg_event_t event;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    // Add the periodic event to the event timer
    evtimer_msg_add_msg(&evtimer, &event, PERIODIC_INTERVAL);

    // Main loop to keep the thread alive
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        event_t *event = (event_t *)msg.content.ptr;
        event->handler(event->arg);
    }

    return 0;
}
