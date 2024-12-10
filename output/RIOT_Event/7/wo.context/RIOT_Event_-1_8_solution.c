#include <xtimer.h>  // For timer related functions
#include <stdio.h>   // For standard input/output functions

// ztimer_t struct for managing timer
static ztimer_t timer;

// Callback function to be called every second
void callback(void *arg) {
    (void)arg; // To avoid unused parameter warning
    printf("Callback called every second!\n");
    // Reset the timer to fire again in 1 second
    ztimer_set(&timer, ZTIMER_MSEC_BASE, 1000); // ZTIMER_MSEC_BASE for millisecond ticks
}

int main(void) {
    // Initialize the timer
    ztimer_init(ZTIMER_MSEC_BASE);
    
    // Set up the timer to call the callback function every second
    timer.callback = callback;
    timer.arg = NULL;
    ztimer_set(&timer, ZTIMER_MSEC_BASE, 1000);
    
    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_MSEC_BASE, 1000);  // Sleep for 1 second to prevent busy-waiting
    }
    return 0;
}