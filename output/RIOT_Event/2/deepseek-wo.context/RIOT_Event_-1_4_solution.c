
#include <stdio.h>
#include "event/periodic.h"
#include "ztimer.h"

// Define the callback function
void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Define the periodic callback event
    event_periodic_callback_t periodic_event;

    // Initialize the periodic callback event
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback, NULL);

    // Set the interval to 1 second (1000000 microseconds)
    event_periodic_callback_set_interval(&periodic_event, 1000000);

    // Start the periodic callback event
    event_periodic_callback_start(&periodic_event);

    // Run the event loop
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
