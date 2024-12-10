#include <xtimer.h>
#include <event/callback.h>
#include <stdio.h>

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered\n");
    // Re-add the callback to ensure it continues every second
    evtimer_add_periodic((evtimer_t *)arg, 1000000U);
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer); // Initialize event timer for message passing

    // Add the first callback to occur after 1 second (1000000 microseconds)
    evtimer_add_periodic(&evtimer, periodic_callback, &evtimer, 1000000U);

    while (1) {
        // Let the event loop run indefinitely
        xtimer_sleep(1);
    }
    return 0;
}