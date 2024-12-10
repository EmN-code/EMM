
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer
static evtimer_t periodic_timer;

// Callback function that will be called every second
static void periodic_callback(void *arg)
{
    (void)arg; // Unused parameter
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&periodic_timer);

    // Set up the periodic event to trigger every second
    evtimer_periodic_event_t event;
    evtimer_periodic_set(&periodic_timer, &event, xtimer_ticks(1 * US_PER_SEC), periodic_callback, NULL);

    // Start the periodic event
    evtimer_periodic_start(&periodic_timer, &event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
