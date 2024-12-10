
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer
static evtimer_t event_timer;

// Define the periodic event
static evtimer_periodic_event_t periodic_event;

// Callback function that will be called every second
void periodic_callback(void *arg) {
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&event_timer);

    // Initialize the periodic event
    evtimer_periodic_event_init(&periodic_event, periodic_callback, NULL, 1000000); // 1 second in microseconds

    // Add the periodic event to the event timer
    evtimer_add_periodic(&event_timer, &periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
