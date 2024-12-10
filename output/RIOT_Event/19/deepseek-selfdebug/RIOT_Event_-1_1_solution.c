
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer
static evtimer_t event_timer;

// Define the event structure
static evtimer_event_t event;

// Callback function to be called every second
void periodic_callback(void *arg) {
    printf("Periodic event triggered!\n");
    // Re-add the event to the timer to repeat every second
    evtimer_add_event(&event_timer, &event, 1000000);
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&event_timer);

    // Initialize the event with the callback function
    evtimer_event_init(&event, periodic_callback, NULL);

    // Add the event to the timer to trigger every second (1000000 microseconds)
    evtimer_add_event(&event_timer, &event, 1000000);

    // Main loop to keep the program running
    while (1) {
        // Yield to allow other threads to run
        xtimer_sleep(1);
    }

    return 0;
}
