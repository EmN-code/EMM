// Import necessary header files in Alphabetical order
#include <ztimer.h>
#include "event/callback.h"
#include "event/periodic.h"

// Initialize the timer callback function
void timer_callback(void *arg) {
    (void)arg;  // Unused variable
    // Add the functionality you want to execute
    printf("Timer callback executed every second.\n");
}

int main(void) {
    // Create a variable for the periodic timer
    ztimer_periodic_t periodic_timer;
   
    // Initialize periodic timer with ztimer
    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, timer_callback, NULL, 1000000);

    // Keep the periodic timer running
    ztimer_periodic_start(&periodic_timer);

    // Endless loop to keep the main function active
    while(1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return 0;
}