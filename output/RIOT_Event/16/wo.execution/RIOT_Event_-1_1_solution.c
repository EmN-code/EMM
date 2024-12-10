#include <stdio.h>
#include <assert.h>
#include "event/periodic.h"
#include "event/callback.h"

void callback(void *arg) {
    printf("Callback executed!\n");
}

int main(void) {
    // Initialize the periodic event
    evtimer_periodic_t evtimer;
    evtimer_init(&evtimer);

    // Add periodic event (every second)
    handler_periodic_periodic_event_t periodic_event;
    periodic_event.callback = callback;
    periodic_event.arg = NULL;
    evtimer_periodic_add(&evtimer, &periodic_event, 1000 * US_PER_MS);

    // Start the event loop
    while (1) {
        evtimer_periodic_sleep(&evtimer, 1000 * US_PER_MS);
    }

    return 0;
}
