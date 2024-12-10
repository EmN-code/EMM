
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic_callback.h"

// Define the callback function
void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Initialize the periodic callback event
    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, &xtimer_clock, &event_queue, periodic_callback, NULL);

    // Set the interval to 1 second
    event_periodic_callback_set_interval(&periodic_event, 1 * US_PER_SEC);

    // Start the periodic callback
    event_periodic_callback_start(&periodic_event);

    // Run the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
