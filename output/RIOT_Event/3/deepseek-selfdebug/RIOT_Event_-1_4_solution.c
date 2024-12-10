
#include <stdio.h>
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic_callback.h"

// Define the callback function
void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_t queue;
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, callback, NULL);

    // Start the periodic callback event with an interval of 1 second
    event_periodic_callback_start(&event, US_PER_SEC);

    // Process events in the queue
    while (1) {
        event_process(&queue);
    }

    return 0;
}
