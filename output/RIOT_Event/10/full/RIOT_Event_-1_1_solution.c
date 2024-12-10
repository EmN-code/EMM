#include "xtimer.h"
#include "event/periodic.h"

static void callback(void *arg) {
    (void)arg;  // Unused
    printf("Callback function executed\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_periodic_t periodic_event;

    // Initialize the periodic event timer
    evtimer_init_msg(&evtimer);

    // Set up the periodic callback every second
    evtimer_periodic_init(&periodic_event, &evtimer, 1 * US_PER_SEC, callback, NULL);

    // Start the periodic event
    evtimer_periodic_start(&periodic_event);

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}