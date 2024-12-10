
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_EVENT_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_event_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Create a periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, &event_queue, (event_handler_t)periodic_event_callback, NULL, PERIODIC_EVENT_INTERVAL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop to process events
    while (1) {
        event_wait(&event_queue);
    }

    return 0;
}
