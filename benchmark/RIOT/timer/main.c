#include <stdio.h>
#include "xtimer.h"

#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds

static xtimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    puts("Timer triggered!");
    // Re-set the timer for periodic execution
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    puts("Timer application started.\n");

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer for the first time
    xtimer_set(&timer, TIMER_INTERVAL);

    // Run indefinitely
    while (1) {
        // Do other tasks here or put the CPU to sleep to save power
        xtimer_sleep(10);  // Sleep for 10 seconds as an example
    }

    return 0;
}
