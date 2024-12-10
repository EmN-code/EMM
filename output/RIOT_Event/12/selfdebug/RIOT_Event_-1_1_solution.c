#include "event/periodic.h"
#include "event/callback.h"
#include <stdio.h>

void my_callback(void *arg) {
    // Print a message every second
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Create an event timer and a callback
    evtimer_t evtimer;
    evtimer_event_t event;

    // Initialize the event timer
    evtimer_init(&evtimer, _evtimer_mbox_handler);

    // Setup callback
    callback_t cb;
    callback_init(&cb, my_callback, NULL);

    // Setup an event that will trigger every 1 second (1000 milliseconds)
    evtimer_event_periodic(&evtimer, &event, &cb, 1 * 1000000);

    // Main loop
    while (1) {
        // Handle events (this would typically be part of the event loop)
    }

    return 0;
}