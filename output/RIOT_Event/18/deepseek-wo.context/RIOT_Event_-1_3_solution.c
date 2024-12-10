
#include "xtimer.h"
#include "event/callback.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static evtimer_t evtimer;
static evtimer_event_t event;

void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&evtimer);

    // Initialize the periodic event
    evtimer_event_init_periodic(&event, xtimer_ticks32(INTERVAL_MS), periodic_callback, NULL);

    // Add the event to the event timer
    evtimer_add_event(&evtimer, &event);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);  // Sleep to prevent busy-waiting
    }

    return 0;
}
