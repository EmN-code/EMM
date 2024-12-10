
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_queue_t event_queue;
    event_periodic_t periodic_event;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, &event_queue, xtimer_ticks64(1 * US_PER_SEC), periodic_callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Run the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
