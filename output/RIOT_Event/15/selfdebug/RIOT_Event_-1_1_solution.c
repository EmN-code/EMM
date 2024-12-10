#include "event/periodic.h"
#include "event/callback.h"
#include "riot_version.h"

#include <stdio.h>

static void my_callback(void *arg) {
    printf("Callback function executed!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Setup the event with the callback and interval of 1 second
    evtimer_periodic_init(&event, &evtimer, my_callback, NULL, 1);

    // Start the event
    evtimer_add(&evtimer, &event);

    // Run in a loop to simulate the system managing events
    while (1) {
        // Handle event (e.g., check timers or incoming interrupts)
        evtimer_run(&evtimer);
    }

    return 0;
}