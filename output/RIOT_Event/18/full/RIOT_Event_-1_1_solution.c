#include <stdlib.h>
#include <stdio.h>
#include "event/periodic.h"

void periodic_callback(void *arg) {
    printf("Callback Function Triggered\n");
}

int main() {
    evtimer_t evtimer;
    evtimer_event_t periodic_event;

    // Initialize the periodic event
    evtimer_init(&evtimer, periodic_callback);

    // Add the event to the event timer
    evtimer_add(&evtimer, &periodic_event, 1000);

    // Start the event timer
    evtimer_start(&evtimer);

    // Main loop
    while (1) {
        // Sleep or do other tasks
        sleep(1);
    }
    
    return 0;
}