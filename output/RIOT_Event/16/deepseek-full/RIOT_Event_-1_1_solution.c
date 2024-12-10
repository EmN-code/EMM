
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer structure
static evtimer_t event_timer;

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
    evtimer_periodic_t periodic_event;
    periodic_event.event.handler = periodic_callback;
    periodic_event.event.arg = NULL;
    periodic_event.interval = (xtimer_ticks32(1 * US_PER_SEC)).ticks; // 1 second interval

    // Add the periodic event to the event timer
    evtimer_add_periodic(&event_timer, &periodic_event);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        xtimer_sleep(1);
    }

    return 0;
}
