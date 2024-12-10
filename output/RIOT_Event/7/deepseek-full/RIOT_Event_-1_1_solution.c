
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer structure
static evtimer_t event_timer;

// Define the periodic event structure
static evtimer_event_t periodic_event;

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

    // Set up the periodic event to trigger every second
    evtimer_add_periodic(&event_timer, &periodic_event, 1000000U, 1000000U, periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Yield to allow other threads to run
        xtimer_sleep(1);
    }

    return 0;
}
