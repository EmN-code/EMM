
#include <stdio.h>
#include "event.h"
#include "ztimer.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback);

    // Set the count to run forever
    event_periodic_callback_set_count(&periodic_event, EVENT_PERIODIC_FOREVER);

    // Set the interval to 1 second
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_MS);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event, INTERVAL_MS);

    // Dispatch events from the queue
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
