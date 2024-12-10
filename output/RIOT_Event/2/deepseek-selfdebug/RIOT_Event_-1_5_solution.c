
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic_callback.h"
#include "event/callback.h"
#include "event/thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t event_queue;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_MSEC, INTERVAL_MS, &event_queue, periodic_callback, NULL);

    // Start the periodic callback event
    event_periodic_callback_start(&event);

    // Run the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
