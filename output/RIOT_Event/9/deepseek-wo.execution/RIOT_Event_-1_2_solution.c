
#include <stdio.h>
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "timex.h"
#include "event/thread.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_mbox_event_t event;
    mbox_t mbox;

    // Initialize the event timer and mbox
    evtimer_init_mbox(&evtimer);
    mbox_init(&mbox, NULL, 0);

    // Set up the event
    event.event.callback = periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer with the specified interval
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Start the periodic event
    ztimer_set(ZTIMER_USEC, &evtimer.timer, PERIODIC_INTERVAL);

    // Main loop to keep the program running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            evtimer_mbox_event_t *received_event = (evtimer_mbox_event_t *)msg.content.ptr;
            received_event->event.callback(received_event->event.arg);
        }
    }

    return 0;
}
