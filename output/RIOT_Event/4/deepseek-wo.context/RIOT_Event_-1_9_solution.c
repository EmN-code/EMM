
#include <stdio.h>
#include "xtimer.h"
#include "event.h"
#include "event/periodic.h"

// Define the event_periodic_callback_t structure
typedef struct {
    event_periodic_t periodic;  /**< periodic event portion */
    event_callback_t event;     /**< callback event portion */
} event_periodic_callback_t;

// Define the callback function
void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, callback, NULL);

    // Start the periodic event with an interval of 1 second
    event_periodic_callback_start(&periodic_event, 1000);

    // Process events
    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
