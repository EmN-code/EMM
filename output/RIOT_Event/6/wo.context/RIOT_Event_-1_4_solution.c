#include <ztimer.h>
#include <stdio.h>

// Timer callback function which will be called periodically
void event_periodic_callback(void *arg) {
    (void)arg;   // Avoid unused parameter warning
    printf("Periodic callback executed.\n");

    // We need to restart the timer
    ztimer_set(ZTIMER_MSEC, (ztimer_t *)arg, 1000);
}

int main(void) {
    // Declare a timer
    ztimer_t periodic_timer;

    // Initialize the timer with callback and the duration
    ztimer_init(ZTIMER_MSEC, &periodic_timer, event_periodic_callback, &periodic_timer, 1000);

    // Start first timer interval
    ztimer_set(ZTIMER_MSEC, &periodic_timer, 1000);

    // Main event loop
    while (1) {
        // Do nothing, CPU can be put to sleep to save power until the timer expires
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}