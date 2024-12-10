
#include "xtimer.h"
#include "event.h"
#include "event/periodic_callback.h"

// Define the callback function
void periodic_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Initialize the periodic callback event
    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Set the interval to 1 second
    event_periodic_callback_set_interval(&periodic_event, 1000);

    // Start the periodic callback
    event_periodic_callback_start(&periodic_event);

    // Dispatch events from the event queue
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
