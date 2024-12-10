
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_t periodic_event;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    periodic_event.handler = periodic_callback;
    periodic_event.arg = NULL;

    // Add the periodic event to the event queue
    xtimer_periodic_event_init(&periodic_event, PERIODIC_INTERVAL);

    // Start the event loop
    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
