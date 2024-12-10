#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (ZTIMER_MSEC_USEC * 1000000U)  // 1 second

// Callback function to be triggered by the timer
static void timer_callback(void *arg) {
    (void)arg;
    printf("Timer callback triggered\n");
}

int main(void) {
    // Initialize the timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the periodic timer
    ztimer_set(ZTIMER_MSEC_USEC, &timer, TIMER_INTERVAL);

    // Enter an infinite loop to keep the program running
    while (1) {
        // Optional: Additional main loop tasks can go here
    }
    return 0;
}