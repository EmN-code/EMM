
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer structure
static evtimer_t event_timer;

// Callback function that will be called every second
void periodic_callback(void *arg) {
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&event_timer);

    // Set up the periodic event to trigger every second
    evtimer_periodic_event_t periodic_event;
    evtimer_add_periodic(&event_timer, &periodic_event, xtimer_ticks(1 * US_PER_SEC), xtimer_ticks(1 * US_PER_SEC), periodic_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        xtimer_sleep(1);
    }

    return 0;
}
