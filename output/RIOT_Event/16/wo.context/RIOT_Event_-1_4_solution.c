#include <ztimer.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

// Callback function to be called periodically
void callback(void *arg) {
    puts("Timer callback executed.");
}

int main(void) {
    // Define a periodic timer
    ztimer_t timer;

    // Setup the timer to trigger every second (1000 milliseconds)
    ztimer_periodic_init(ZTIMER_MSEC, &timer, 1000, callback, NULL);

    while (1) {
        // Wait here until the timer period elapses
        ztimer_sleep(ZTIMER_MSEC, 1000);
        ztimer_periodic_wakeup(ZTIMER_MSEC, &timer);
    }

    return 0;
}