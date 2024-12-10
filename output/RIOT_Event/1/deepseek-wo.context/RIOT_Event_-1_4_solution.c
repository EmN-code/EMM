
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_periodic_t periodic_event;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, xtimer_clock_ztimer64, &event_queue, periodic_callback, NULL);

    // Set the interval for the periodic event
    event_periodic_set_interval(&periodic_event, INTERVAL_MS);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Run the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
