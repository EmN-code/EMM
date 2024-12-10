#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

// Callback function for the periodic event
void periodic_callback(void *arg) {
    printf("Periodic callback triggered\n");
}

int main(void) {
    kernel_pid_t pid = thread_getpid();

    // Initialize a periodic timer
    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    // Define the periodic event and add to timer
    evtimer_periodic_event_t periodic_event;
    periodic_event.interval = 1; // set period to 1 second
    periodic_event.callback = periodic_callback;

    // Add the periodic event to the evtimer
    evtimer_add_periodic(&evtimer, &periodic_event);

    // Start periodic timer
    evtimer_run(&evtimer);

    // Keep the main thread alive
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}