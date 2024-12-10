
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Initialize the periodic event
    periodic_event_t periodic_event;
    periodic_event_init(&periodic_event, &event_queue, periodic_callback, NULL);

    // Set the periodic event to trigger every second
    xtimer_periodic_event_init(&periodic_event, PERIODIC_INTERVAL);

    // Start the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
