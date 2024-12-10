#include <xtimer.h>
#include <stdio.h>
#include <event.h>
#include <evtimer.h>

#define PERIODIC_INTERVAL (1U * US_PER_SEC)

static evtimer_t evtimer;
static evtimer_event_t periodic_event;

static void callback(void *arg) {
    (void)arg; // Argument not used
    printf("Periodic event triggered\n");
    evtimer_add(&evtimer, &periodic_event);
}

int main(void) {
    printf("System started.\n");

    // Initialize the event timer
    evtimer_init(&evtimer, EVENT_PRIO_MEDIUM);

    // Set up the periodic event
    evtimer_set(&periodic_event, callback, NULL);

    // Add the event to the timer with the periodic interval
    evtimer_add(&evtimer, &periodic_event.event, PERIODIC_INTERVAL);

    // Run the event dispatcher loop
    event_dispatch(EVENT_PRIO_MEDIUM);

    // Should never be reached
    return 0;
}
