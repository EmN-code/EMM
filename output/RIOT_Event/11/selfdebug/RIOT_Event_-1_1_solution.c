#include <stdio.h>
#include <event/periodic.h>
#include <event/callback.h>

void my_periodic_callback(void *arg) {
    (void)arg; // avoid unused parameter warning
    printf("Periodic callback triggered\n");
}

int main() {
    evtimer_t evtimer;
    evtimer_periodic_event_t periodic_event;

    // Initialize the periodic event structure
    evtimer_periodic_init(&periodic_event);
    evtimer_periodic_set_period(&periodic_event, 1000000); // 1000000 microseconds = 1 second
    evtimer_periodic_set_callback(&periodic_event, my_periodic_callback, NULL);

    // Initialize event timer
    evtimer_init(&evtimer);

    // Start the periodic timer
    evtimer_periodic_add(&evtimer, &periodic_event);

    // Main loop
    while (1) {
        event_periodic_handle_periodic_events();
    }
    return 0;
}