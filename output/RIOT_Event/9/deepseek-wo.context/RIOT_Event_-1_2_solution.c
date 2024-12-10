
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_queue_t event_queue;
    event_periodic_t periodic_event;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, &event_queue, xtimer_now_usec64(), PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Run the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
