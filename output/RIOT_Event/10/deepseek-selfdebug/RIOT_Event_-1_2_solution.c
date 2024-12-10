
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic_callback.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered\n");
}

int main(void) {
    event_periodic_callback_t periodic_event;

    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, xtimer_ticks32(INTERVAL_MS), periodic_callback, NULL);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
