
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;
    event_queue_t event_queue;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, &event_queue, periodic_callback, NULL, PERIODIC_INTERVAL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop to keep the application running
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
