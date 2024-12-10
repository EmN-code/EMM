#include <ztimer.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC) // Define the interval for the timer in microseconds.

static ztimer_t timer; // Declare a ztimer_t structure to hold timer details.

// Callback function to be triggered every second by the timer.
void event_periodic(void *arg) {
    (void)arg;
    printf("Periodic Event Triggered!\n");
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL); // Reschedule the timer.
}

int main(void) {
    puts("RIOT Timer with Event Periodic Example.");
    
    // Initialize the timer with the defined interval and set the callback function.
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
    timer.callback = event_periodic;
    timer.arg = NULL;

    // Start the timer loop.
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL); // Sleep until the next interval.
    }

    return 0;
}