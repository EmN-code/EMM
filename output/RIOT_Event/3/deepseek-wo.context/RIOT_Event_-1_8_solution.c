
#include "event/callback.h"
#include "ztimer.h"
#include "xtimer.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t periodic_event;
    event_queue_t event_queue;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic callback event
    event_periodic_callback_create(&periodic_event, ZTIMER_MSEC, INTERVAL_MS, &event_queue, periodic_callback, NULL);

    // Start the periodic callback event
    event_periodic_callback_start(&periodic_event, INTERVAL_MS);

    // Dispatch events from the event queue
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
