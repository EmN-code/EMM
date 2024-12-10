#include <xtimer.h>
#include <stdio.h>
#include "event/periodic.h"

static void periodic_event_handler(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_periodic_event_t periodic_event;

    // Initialize the periodic event
    evtimer_periodic_event_init(&periodic_event,
                                periodic_event_handler,
                                NULL,
                                1 * US_PER_SEC); // 1 second interval

    // Initialize the event timer for periodic events
    evtimer_periodic_init(&evtimer);

    // Add the periodic event to the event timer
    evtimer_periodic_add(&evtimer, &periodic_event);

    while (1) {
        // Sleep for a short time to allow the periodic event to trigger
        xtimer_sleep(1);
    }
    return 0;
}