
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer structure
static evtimer_t event_timer;

// Define the periodic event structure
static evtimer_periodic_event_t periodic_event;

// Callback function that will be called every second
void periodic_callback(void *arg)
{
    (void)arg; // Unused parameter
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&event_timer);

    // Initialize the periodic event
    evtimer_periodic_event_init(&periodic_event, periodic_callback, NULL);

    // Add the periodic event to the event timer with a period of 1 second
    evtimer_add_periodic(&event_timer, &periodic_event, 1 * US_PER_SEC);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        xtimer_sleep(1);
    }

    return 0;
}
