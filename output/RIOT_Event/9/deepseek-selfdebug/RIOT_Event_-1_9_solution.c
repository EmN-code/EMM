
#include "xtimer.h"
#include "event/callback.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    event_periodic_t periodic_event;

    // Initialize the periodic event
    event_periodic_init(&periodic_event, &timer, periodic_callback, NULL);

    // Set the interval for the periodic event
    event_periodic_set(&periodic_event, INTERVAL_MS);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);  // Sleep to prevent busy-waiting
    }

    return 0;
}
