
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

// Define the callback function
void periodic_callback(void *arg) {
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks32(1 * US_PER_SEC), periodic_callback, NULL);

    // Add the event to the periodic event handler
    event_periodic_add_event(&periodic_event);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
