// Description:
// This code initializes an event timer with a periodic callback function that triggers every second. 
// We use the RIOT OS's ztimer module to set up and manage the timer. The timer is initialized
// and set to trigger the specified callback function periodically.

// Imports
#include <ztimer.h>
#include <stdio.h>
#include <event.h>

void event_periodic(void *arg) {
    puts("Callback function triggered.");
}

int main(void) {
    // Initialize timer
    ztimer_init(ZTIMER_USEC, xtimer_usec_from_sec(1));
    
    // Set up the periodic event that triggers every second
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, event_periodic, NULL, 1 * US_PER_SEC);
    ztimer_periodic(&periodic_timer);

    puts("Timer started.");

    while (1) {
        // Keep the program running to continue handling the timer events
        ztimer_spin(ZTIMER_USEC, US_PER_SEC);
    }
    
    return 0;
}