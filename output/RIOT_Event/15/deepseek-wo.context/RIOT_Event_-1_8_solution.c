
#include "xtimer.h"
#include "event/periodic.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    xtimer_t timer;

    // Initialize the periodic event
    event_periodic_init(&periodic_event, &timer, periodic_callback, NULL);

    // Set the periodic event to trigger every INTERVAL_MS milliseconds
    xtimer_periodic_set(&timer, INTERVAL_MS);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);  // Sleep to prevent busy-waiting
    }

    return 0;
}
