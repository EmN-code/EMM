
#include <stdio.h>  // Include stdio.h to resolve the implicit declaration of 'puts'
#include "event/periodic.h"
#include "ztimer.h"

#define INTERVAL (1 * ZTIMER_SEC)  // Define the interval for the periodic callback

static void periodic_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    puts("Periodic callback triggered!");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, periodic_callback, NULL);

    // Set the interval for the periodic callback
    event_periodic_callback_set_interval(&event, INTERVAL);

    // Start the periodic callback
    event_periodic_callback_start(&event);

    // Run the event loop
    while (1) {
        event_loop(&queue);
    }

    return 0;
}
