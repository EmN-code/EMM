#include "ztimer.h"
#include "stdio.h"

#define INTERVAL (1U * ZTIMER_MSEC)  // 1 second interval

static ztimer_t timer;

void callback(void *arg) {
    (void)arg;
    puts("Callback function called!");
    ztimer_set(&ztimer, &timer, INTERVAL);
}

int main(void) {
    // Initialize timer
    ztimer_set(&ztimer, &timer, INTERVAL);
    timer.callback = callback;
    timer.arg = NULL;

    // Start the callback function by setting the initial timer
    ztimer_set(&ztimer, &timer, INTERVAL);

    // Main loop
    while (1) {
        // Perform other tasks here
        pause();  // Suspend execution
    }

    return 0;
}