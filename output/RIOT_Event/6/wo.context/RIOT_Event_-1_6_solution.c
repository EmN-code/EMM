#include <ztimer.h>  // Assuming ztimer is the correct timer implementation
#include <stdio.h>

// Time in microseconds for a 1 second timer interval
#define TIMER_INTERVAL (1U * 1000000U)  // 1 second in microseconds

// Callback function that will be called every second
void callback(void *arg) {
    printf("Timer expired, calling callback function.\n");
}

int main(void) {
    ztimer_t timer;
    ztimer_init();  // Initialize the ztimer module if needed (depends on specific platform/implementation)

    // Set the callback function for the timer
    timer.callback = callback;
    timer.arg = NULL;

    // Start the timer with the specified interval
    ztimer_set(&timer, ZTIMER_USEC, TIMER_INTERVAL);

    // Main loop or thread activity goes here
    while (1) {
        // Simulate processing or other activities
    }

    return 0;
}