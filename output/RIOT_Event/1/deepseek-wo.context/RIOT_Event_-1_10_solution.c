
#include <stdio.h>
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "event.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_queue_t queue;
    event_periodic_callback_t periodic_event;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &queue, periodic_callback, NULL);

    // Start the periodic callback event with a 1 second interval
    event_periodic_callback_start(&periodic_event, INTERVAL_MS);

    // Run the event loop
    while (1) {
        event_loop(&queue);
    }

    return 0;
}
