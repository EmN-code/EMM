#include <ztimer.h>  // correct header for event and timer structures
#include <stdio.h>  // standard I/O library for printf function

#define TIMER_INTERVAL (1U * US_PER_SEC)  // define the timer interval to 1 second

static ztimer_t timer;  // define a ztimer_t variable to manage the timer

// Callback function triggered by the timer
static void timer_callback(void *arg) {
    (void)arg;  // explicitly ignore the unused parameter warning
    printf("Timer callback executed\n");  // print message when callback function is executed
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);
}

int main(void) {
    printf("Starting periodic event example...\n");  // print initial message

    // Initialize the ztimer
    timer.callback = timer_callback;  // set the callback function
    timer.arg = NULL;  // set the argument that will be passed to the callback

    // Start the timer with the specified interval
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);

    while (1) {
        // Keep the main function running
    }

    return 0;
}