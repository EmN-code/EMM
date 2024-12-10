#include <ztimer.h>
#include <stdio.h>

#define INTERVAL (1U * ZTIMER_MSEC)  // Define interval for 1 second

static ztimer_t timer;

void callback(void *arg) {
    printf("Callback function triggered\n");
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL); // Reschedule the timer
}

int main(void) {
    // Initialize the timer structure
    timer.callback = callback;
    timer.arg = NULL;

    // Start the timer
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
    
    while (1) {
        // Main loop does nothing, timer will handle the callback
    }
    return 0;
}