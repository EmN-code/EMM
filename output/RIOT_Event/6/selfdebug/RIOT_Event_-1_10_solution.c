#include "ztimer.h"

/**
 * A callback function to be executed every second.
 *
 * @param arg       A pointer to argument (unused in this example).
 */
void event_periodic(void *arg) {
    (void)arg; // Suppress unused parameter warning
    // Insert the periodic task to be executed here
    puts("Periodic event triggered.");
}

int main(void) {
    // Timer setup
    ztimer_t timer;
    timer.callback = event_periodic;
    timer.arg = NULL;

    // Set the timer to trigger every second (i.e., 1000 milliseconds)
    int interval_ticks = ztimer_usec_from_ms(1000);
    ztimer_start_from_now(ZTIMER_PERIODIC, &timer, interval_ticks);

    // Enter a loop to keep the program running
    while (1) {
        // Do nothing, just wait for timer events
    }

    return 0;
}