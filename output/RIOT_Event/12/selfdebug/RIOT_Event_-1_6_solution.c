#include <ztimer.h>  // ztimer provides timer and periodic timer functionality
#include <stdio.h>   // Standard Input/Output library for printf function
#include <inttypes.h>  // Standard integer types
#include <event/periodic.h> // Necessary for evtimer and periodic events

// Function to be called by the periodic timer
void callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Periodic timer callback triggered\n");
}

int main(void) {
    // Initialize a periodic timer
    ztimer_periodic_t my_timer;
    ztimer_periodic_init(ZTIMER_SEC, &my_timer, callback, NULL, 1);
    
    // Start a periodic timer that will wake up every second
    ztimer_periodic_start(&my_timer);
    
    while(1) {
        // This call will block and wait until the next timer event
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0; // Return 0 to indicate successful execution
}