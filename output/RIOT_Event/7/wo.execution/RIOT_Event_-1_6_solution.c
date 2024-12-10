#include "xtimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time_units.h"
#include "msg.h"
#include "timex.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Define an event handler function for the evtimer
void event_handler(void *arg) {
    (void)arg; // suppress unused parameter warning
    printf("Event fired!\n");
}

int main(void) {
    // Define an evtimer and an event
    evtimer_msg_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t target_pid = thread_getpid();

    printf("Initializing the event timer...\n");

    // Initialize evtimer_msg
    evtimer_msg_init_queue(&evtimer, NULL, 1);

    // Add the event to the evtimer
    event.event.handler = event_handler;
    evtimer_add_msg(&evtimer, &event, target_pid);

    printf("Starting the event timer...\n");

    // This loop dispatches events for every second
    while (1) {
        evtimer_periodic_start_usec(&evtimer, 1000000);
        evtimer_dispatch(&evtimer);
    }

    return 0;
}