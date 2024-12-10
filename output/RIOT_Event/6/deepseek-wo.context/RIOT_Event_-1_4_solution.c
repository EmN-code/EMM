
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
    xtimer_t periodic_timer;

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_init(&periodic_event, periodic_callback, NULL);

    // Initialize the periodic timer
    xtimer_init(&periodic_timer);

    // Set the periodic timer to trigger the event every second
    xtimer_periodic_wakeup(&periodic_timer, &periodic_event, PERIODIC_INTERVAL);

    // Main loop to wait for events
    while (1) {
        event_wait(&event_queue, &periodic_event);
    }

    return 0;
}
