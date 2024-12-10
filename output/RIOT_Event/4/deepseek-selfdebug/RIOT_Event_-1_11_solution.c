
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL_SEC (1 * US_PER_SEC)  // 1 second interval

static void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, callback, NULL);

    // Set the interval for the periodic callback (1 second)
    event_periodic_callback_set_interval(&event, INTERVAL_SEC);

    // Start the periodic callback
    event_periodic_callback_start(&event);

    // Dispatch events from the queue
    while (1) {
        event_queue_dispatch(&queue);
    }

    return 0;
}
