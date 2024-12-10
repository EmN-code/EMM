
#include <stdio.h>
#include "event/periodic_callback.h"  // Include the correct header for periodic callbacks
#include "ztimer.h"
#include "thread.h"

// Define the event_periodic_callback_t structure
event_periodic_callback_t periodic_event;

// Callback function to be executed periodically
void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, NULL, periodic_callback, NULL);

    // Set the period to 1 second
    event_periodic_callback_set_period(&periodic_event, 1000);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
