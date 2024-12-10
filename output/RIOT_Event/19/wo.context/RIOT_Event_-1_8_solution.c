#include "ztimer.h"
#include "msg.h"
#include <stdio.h>

// Callback function to be called every second
void event_periodic(void *arg) {
    puts("Callback function executed!");
    // Re-set the timer for periodic execution
    ztimer_periodic_t *timer = (ztimer_periodic_t*)arg;
    ztimer_periodic_start(timer, 1 * ZTIMER_MSEC_PER_SEC);
}

int main(void) {
    // Initialize a periodic timer
    ztimer_clock_t *clock = ztimer_clock_register(ZTIMER_MSEC);
    if (clock == NULL) {
        puts("Failed to register ztimer clock");
        return 1;
    }

    ztimer_periodic_t timer;
    ztimer_periodic_init(clock, &timer, event_periodic, &timer);

    // Start the periodic timer for the first time
    ztimer_periodic_start(&timer, 1 * ZTIMER_MSEC_PER_SEC);

    while (1) {
        // Idle loop
    }
    return 0;
}