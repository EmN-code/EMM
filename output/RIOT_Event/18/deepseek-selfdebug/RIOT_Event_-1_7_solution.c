
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_t periodic_event;
static event_queue_t event_queue;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, xtimer_ticks32(PERIODIC_INTERVAL), periodic_callback, NULL);

    // Add the periodic event to the event queue
    event_periodic_add_event(&event_queue, &periodic_event);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Wait for events
    while (1) {
        event_wait(&event_queue);
    }

    return 0;
}
