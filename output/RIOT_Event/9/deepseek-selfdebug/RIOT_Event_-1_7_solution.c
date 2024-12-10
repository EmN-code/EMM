
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void event_periodic_callback(void *arg) {
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event periodic timer
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, event_periodic_callback, NULL);

    // Set the periodic event to trigger every second
    event_periodic_set(&periodic_event, xtimer_now_usec(), TIMER_INTERVAL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
