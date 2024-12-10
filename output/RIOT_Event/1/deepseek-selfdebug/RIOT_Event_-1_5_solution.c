
#include <stdio.h>
#include "event.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t periodic_event;
    event_queue_t event_queue;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Set the interval for the periodic callback
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_MS);

    // Start the periodic callback
    event_periodic_callback_start(&periodic_event);

    // Run the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
