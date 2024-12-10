#include <stdio.h>
#include "xtimer.h" // For handling timers
#include "timex.h"  // For handling time values and conversions

// Global timer variable
static xtimer_t timer;

// Callback function to be executed periodically
void periodic_callback(void *arg) {
    (void)arg;
    printf("Callback executed\n");
    
    // Restart the timer to trigger the next callback in 1 second
    xtimer_set(&timer, xtimer_usec_from_sec(1));
}

int main(void) {
    printf("Starting periodic timer example\n");

    // Initialize the timer
    timer.callback = periodic_callback;
    timer.arg = NULL;
    timer.target = timer.long_target = XTIMER_USEC(1000000); // Set target for 1 second

    // Start the timer
    xtimer_set(&timer, timer.long_target);

    return 0;
}