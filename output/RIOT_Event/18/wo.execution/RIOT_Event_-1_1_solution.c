#include <timex.h>
#include <stdio.h>
#include <event/timeout.h>
#include <event/callback.h>
#include <event/periodic.h>

static void callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    printf("Starting periodic event example.\n");
    
    evtimer_periodic_t periodic;
    evtimer_t evtimer;

    // Initialize event timer with a callback
    evtimer_init(&evtimer, callback);

    // Set the periodic event for every second (1000 milliseconds)
    evtimer_periodic_init(&periodic, &evtimer, 1000);

    // Start the periodic event
    evtimer_periodic_start(&periodic, &evtimer);

    // Main loop can continue with other tasks
    while (1) {
        // Sleep or perform other tasks
    }

    return 0;
}