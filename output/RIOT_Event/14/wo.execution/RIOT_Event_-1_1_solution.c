#include <xtimer.h>
#include <stdio.h>
#include "event/callback.h"

// Callback function to be triggered every second
void my_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    printf("Callback triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;

    // Initialize the event timer for periodic callback
    evtimer_init(&evtimer, _evtimer_msg_handler);

    // Setup timer parameters
    evtimer_add_periodic(&evtimer, &event, 1 * US_PER_SEC, my_callback, NULL);

    // Start the event loop
    while (1) {
        evtimer_periodic(&evtimer);
        xtimer_sleep(1); // Sleep to simulate periodic execution
    }

    return 0;
}