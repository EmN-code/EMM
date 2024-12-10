
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "event/thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t queue;
    event_periodic_callback_t periodic_event;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &queue, periodic_callback, NULL);

    // Set the interval for the periodic event
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_MS);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Dispatch events from the queue
    while (1) {
        event_queue_dispatch(&queue);
    }

    return 0;
}
