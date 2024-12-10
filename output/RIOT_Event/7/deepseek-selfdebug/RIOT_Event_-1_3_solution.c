
#include <stdio.h>
#include "xtimer.h"
#include "evtimer.h"
#include "event.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static evtimer_t periodic_timer;
static event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic event triggered!\n");
    evtimer_add(&periodic_timer, &periodic_event);
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&periodic_timer, periodic_callback, NULL);

    // Initialize the event
    periodic_event.handler = periodic_callback;
    periodic_event.arg = NULL;

    // Add the event to the timer with the specified interval
    evtimer_add_msg(&periodic_timer, &periodic_event, INTERVAL_MS);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
