
#include <stdio.h>  // Include stdio.h for puts function
#include "event.h"  // Include event.h for event_periodic_callback_t and related functions
#include "ztimer.h" // Include ztimer.h for ztimer_clock_t

// Define the event_periodic_callback_t structure
typedef struct {
    event_periodic_t periodic;  // Periodic event portion
    event_callback_t event;     // Callback event portion
} event_periodic_callback_t;

// Define the callback function
void callback(void *arg) {
    puts("Event triggered!");
}

int main(void) {
    // Initialize the event queue
    event_queue_t queue;
    event_queue_init(&queue);

    // Initialize the event_periodic_callback_t structure
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, callback);

    // Start the periodic event with an interval of 1 second (1000000 microseconds)
    event_periodic_callback_start(&event, 1000000);

    // Process events in the queue
    while (1) {
        event_process(&queue);
    }

    return 0;
}
