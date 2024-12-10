#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>

// Define the timer interval (1 second = 1,000,000 microseconds)
#define TIMER_INTERVAL (1U * US_PER_SEC)

// Define a timer
static xtimer_t my_timer;

// Callback function for the timer
void timer_callback(void *arg) {
    (void)arg; // We don't use this argument
    printf("Timer triggered!\n");

    // Reset the timer to trigger again in 1 second
    xtimer_set(&my_timer, TIMER_INTERVAL);
}

int main(void) {
    printf("Starting periodic timer example...\n");

    // Initialize timer fields
    my_timer.callback = timer_callback;
    my_timer.arg = NULL;

    // Set and start the timer for the first time
    xtimer_set(&my_timer, TIMER_INTERVAL);

    // Main thread can do other work here
    while (1) {
        // For demonstration, let the main thread sleep
        xtimer_sleep(5);
        printf("Main thread is doing other tasks.\n");
    }
    return 0;
}