#include "ztimer.h" // Ensure this is included for ztimer functionality
#include <stdio.h>    

#define INTERVAL (1U * US_PER_SEC) // set the time interval to 1 second

// Callback function that will be called every interval
void callback(void *arg) {
    puts("Callback triggered!");
}

int main(void) {
    ztimer_periodic_t periodic_timer;
    
    // Initialize the periodic timer with the callback and interval
    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, callback, NULL, INTERVAL);
    
    // Start the timer
    ztimer_periodic_start(&periodic_timer, INTERVAL);

    // Continue running...
    while (1) {
        // Optional: perform other tasks here or put the CPU to sleep
    }

    return 0;
}