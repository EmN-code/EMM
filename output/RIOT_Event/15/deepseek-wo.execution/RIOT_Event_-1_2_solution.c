
#include <stdio.h>
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "evtimer_msg.h"

// Define the event timer and periodic event structure
static evtimer_t evtimer;
static evtimer_mbox_event_t periodic_event;

// Callback function to be called every second
static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set the periodic event to trigger every second
    periodic_event.offset = 0;
    periodic_event.interval = ZTIMER_SEC(1);
    periodic_event.event.handler = periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the periodic event to the event timer
    evtimer_add_event(&evtimer, &periodic_event.event, periodic_event.offset);

    // Start the event timer
    while (1) {
        evtimer_mbox_event_t *event = evtimer_mbox_event_get(&evtimer);
        if (event) {
            event->event.handler(event->event.arg);
            evtimer_add_event(&evtimer, &event->event, event->interval);
        }
    }

    return 0;
}
