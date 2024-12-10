
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_queue_t event_queue;
    event_t periodic_event;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event, &event_queue);

    // Main loop to process events
    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
