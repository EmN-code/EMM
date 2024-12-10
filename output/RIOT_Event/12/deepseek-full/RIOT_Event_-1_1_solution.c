
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer structure
static evtimer_t event_timer;

// Define the event structure for periodic events
static evtimer_event_t periodic_event;

// Callback function that will be called every second
void periodic_callback(void *arg) {
    printf("Periodic event triggered!\n");
    // Re-add the event to the timer to repeat every second
    evtimer_add_event_periodic(&event_timer, &periodic_event, 1000000U);
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&event_timer);

    // Initialize the periodic event
    periodic_event.event.handler = (event_handler_t)periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the event to the timer to trigger every second (1000000 microseconds)
    evtimer_add_event_periodic(&event_timer, &periodic_event, 1000000U);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        xtimer_sleep(1);
    }

    return 0;
}
