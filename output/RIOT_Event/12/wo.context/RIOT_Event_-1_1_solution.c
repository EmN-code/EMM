#include "event/periodic.h"  // For using evtimer and periodic events
#include "event/callback.h"  // For defining and setting the callback function
#include <stdio.h>  // For printf function

void my_callback(void *arg) {
    printf("Callback function triggered!\n");
}

int main() {
    evtimer_t evtimer;
    evtimer_periodic_event_t periodic_event;

    // Initialize the event timer for periodic events
    evtimer_init(&evtimer, NULL);

    // Set up the periodic event with a 1-second interval
    evtimer_periodic_init(&periodic_event, &evtimer, my_callback, NULL, 1 * US_PER_SEC);

    // Start the periodic event
    evtimer_periodic_start(&periodic_event);

    // Main loop
    while (1) {
        // You could do other tasks here
        // Process events
        evtimer_periodic_dispatch(&evtimer);
    }

    return 0;
}