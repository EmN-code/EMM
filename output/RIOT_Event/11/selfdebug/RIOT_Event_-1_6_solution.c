#include <ztimer.h>  // Ensure this header is included for ztimer functionalities
#include <stdio.h>

// Callback function to be called by the timer
void example_callback(void *arg) {
    printf("Callback executed!\n");
}

int main(void) {
    ztimer_periodic_t periodic_timer;  // Define the periodic timer object
    
    // Initialize the periodic timer with the callback function, interval is set to 1 second
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, example_callback, NULL, 1000);
    
    // Start the timer
    ztimer_periodic_start(&periodic_timer);

    while (1) {
        // The main loop can continue executing other tasks
    }
    return 0;
}