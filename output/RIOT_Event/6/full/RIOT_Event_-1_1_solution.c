#include "event/periodic.h" // For the periodic events
#include "xtimer.h" // For xtimer_sleep functionality
#include "kernel.h" // Basic kernel functions

// Callback function that gets called every second
void my_timer_callback(void *arg) {
    (void)arg; // We don't use this parameter
    printf("Callback function called every second!\n");
}

int main(void) {
    // Initialize a periodic event
    evtimer_t evtimer;
    evtimer_event_t event;
    event.callback = my_timer_callback;
    event.arg = NULL; // No argument is passed to the callback

    // Initialize the event timer with the periodic handler
    evtimer_init(&evtimer, &etvimer_periodic_handler);

    // Add the event with a period of 1 second (1000 milliseconds)
    evtimer_add_periodic(&evtimer, &event, 1000 * MSEC);

    // Start the event timer
    evtimer_start(&evtimer);

    // Busy loop (simulating main application loop)
    while (1) {
        // Sleep for a shorter period to yield to the callback function
        xtimer_sleep(1);
    }

    return 0;
}