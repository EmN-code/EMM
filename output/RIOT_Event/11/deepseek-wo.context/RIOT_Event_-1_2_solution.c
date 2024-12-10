
#include "xtimer.h"
#include "event/periodic.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;

    // Initialize the periodic event with the callback function and interval
    event_periodic_init(&periodic_event, xtimer_ticks32(INTERVAL_MS), periodic_callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        xtimer_sleep(1);
    }

    return 0;
}
