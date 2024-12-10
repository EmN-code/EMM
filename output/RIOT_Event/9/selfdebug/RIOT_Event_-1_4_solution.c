#include "ztimer.h"
#include "net/af.h"
#include "event/periodic.h"
#include <stdio.h>

ztimer_t timer;

void callback(void *arg) {
    puts("Callback function called");
}

int main(void) {
    // Initialize periodic event
    evtimer_periodic_event_t periodic_event;
    evtimer_periodic_init(&timer, callback, NULL);

    // Set timer to trigger callback every second
    ztimer_set(&timer, 1000);

    // Start the periodic event
    evtimer_periodic_start(&periodic_event);

    while (1) {
        // Main loop can perform other tasks
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}