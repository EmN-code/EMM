
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the callback function
void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the periodic event timer
    periodic_event_t periodic_event;
    periodic_event_init(&periodic_event, periodic_callback, NULL);

    // Set the period to 1 second
    xtimer_ticks32_t period = xtimer_ticks_from_usec(1000000);

    // Start the periodic event timer
    periodic_event_add(&periodic_event, period);

    // Main loop to keep the program running
    while (1) {
        // Process events (this is a placeholder, as the periodic event is handled by the timer)
        xtimer_sleep(1);
    }

    return 0;
}
