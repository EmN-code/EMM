#include <event/periodic.h>
#include <event/callback.h>
#include "xtimer.h" // For delay and timer functions

void event_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Event triggered every second!\n");
}

int main(void) {
    // Delay to allow proper system initialization
    xtimer_sleep(1);
    
    // Declare an event and initialize it with the callback function
    event_timer_t event_timer;
    event_periodic_init(&event_timer, event_callback, NULL);
    
    // Set the timer to trigger every second (1Hz)
    event_periodic_start(&event_timer, 1 * US_PER_SEC);

    // Main loop, just idle
    while (1) {
        xtimer_sleep(1); // Sleep for a short period to reduce CPU usage
    }

    return 0;
}