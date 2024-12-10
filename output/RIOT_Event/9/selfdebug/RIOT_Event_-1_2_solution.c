#include <stdio.h>
#include <stdlib.h>
#include <ztimer.h> // ztimer should be included for timer functions
#include <event/callback.h> // Include the correct header for event callbacks
#include <event.h> // For evtimer_t and related types

// Custom handler function for the event timer
void event_handler(void *arg) {
    printf("Event triggered!\n");
    // Add any additional functionality you need on each event
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;
    callback_t callback;

    // Initialize the event timer
    evtimer_init(&evtimer, _evtimer_mbox_handler);

    // Initialize the callback
    callback_init(&callback, event_handler, NULL);

    // Setup an event to trigger periodically every second
    evtimer_event_periodic(&evtimer, &event, &callback, ztimer_now(ZTIMER_MSEC) + 1000);

    printf("Event timer started.\n");

    // Main loop
    while (1) {
        // Sleep or perform other tasks
    }
    return 0;
}