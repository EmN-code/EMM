
#include <stdio.h>
#include "xtimer.h"
#include "evtimer.h"

// Define the event timer and event structures
evtimer_t evtimer;
evtimer_event_t event;

// Callback function to be called every second
void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&evtimer);

    // Initialize the event with the callback function and interval
    evtimer_event_init_periodic(&event, xtimer_ticks_from_usec(1000000), periodic_callback, NULL);

    // Add the event to the event timer
    evtimer_add_event(&evtimer, &event);

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
