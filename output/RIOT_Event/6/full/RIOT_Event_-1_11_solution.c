// This code demonstrates the initialization and use of an event timer to periodically trigger a callback function every second.

#include "event/callback.h"
#include "event.h"

// Initialize and define the callback function
void callback(void *arg) {
    // Code to execute on each callback
    (void)arg; // Suppress unused parameter warning
}

int main(void) {
    // Create an instance of evtimer_t
    evtimer_t evtimer;
    
    // Initialize the evtimer to use the mailbox handler
    evtimer_init_mbox(&evtimer);
    
    // Define the event to be added
    evtimer_event_t my_event;
    my_event.period = 1 * US_PER_SEC; // Set the period to 1 second
    my_event.callback = callback;
    my_event.arg = NULL;
    
    // Add the event to the event timer
    evtimer_add(&evtimer, &my_event);

    // Start the event timer
    evtimer_run(&evtimer);

    return 0;
}