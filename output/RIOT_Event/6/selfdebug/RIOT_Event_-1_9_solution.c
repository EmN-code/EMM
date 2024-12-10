#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

// callback function for the timer
void callback(void *arg) {
    (void)arg;
    puts("Periodic task executed");
}

int main(void) {
    // ztimer is used for managing periodic events
    ztimer_t timer;
    timer.callback = callback;
    timer.arg = NULL; // No arguments passed to callback

    // Set up a periodic timer
    ztimer_periodic(&timer, INTERVAL);

    // Blink loop
    while (1) {
        // Timer and callback will keep running every second.
        ztimer_periodic(&timer, INTERVAL);

        // Sleep to allow other tasks to run
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
    return 0;
}