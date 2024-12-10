
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer structure
static evtimer_t periodic_timer;

// Callback function that will be called every second
void periodic_callback(void *arg)
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
    evtimer_add_periodic(&periodic_timer, &event, xtimer_ticks64(xtimer_s_to_ticks(1)), xtimer_ticks64(xtimer_s_to_ticks(1)), periodic_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        xtimer_sleep(1);
    }

    return 0;
}
