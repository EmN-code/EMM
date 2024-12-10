
#include <stdio.h>
#include <xtimer.h>
#include <event/periodic.h>

// Define the event timer
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
    evtimer_periodic_init(&periodic_event, periodic_callback, NULL);

    // Set the periodic event to trigger every second
    evtimer_periodic_add(&event_timer, &periodic_event, 1000000U);

    // Main loop to keep the program running
    while (1) {
        // Process events
        evtimer_process(&event_timer);
        xtimer_usleep(1000); // Sleep for 1ms to avoid busy-waiting
    }

    return 0;
}
