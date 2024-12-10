
#include <stdio.h>
#include "xtimer.h"
#include "evtimer.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static evtimer_event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&periodic_timer);

    // Set up the periodic event
    periodic_event.event.offset = 0;
    periodic_event.event.handler = periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the periodic event to the timer
    evtimer_add_periodic(&periodic_timer, &periodic_event, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
