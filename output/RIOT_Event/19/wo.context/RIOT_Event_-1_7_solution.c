#include <ztimer.h>
#include <stdio.h>

void periodic_event_callback(void *arg) {
    printf("Callback function invoked\n");
    // Re-set the timer for the next callback
    ztimer_tick_t now = ztimer_now(ZTIMER_MSEC);
    ztimer_set(ZTIMER_MSEC, (ztimer_t *)arg, now + 1000);
}

int main(void) {
    ztimer_t timer;
    // Set the callback and argument
    timer.callback = periodic_event_callback;
    timer.arg = &timer;

    // Start the timer
    ztimer_set(ZTIMER_MSEC, &timer, ztimer_now(ZTIMER_MSEC) + 1000);

    // Keeping the main function running
    while (1) {
        // Add a delay to reduce CPU usage
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}