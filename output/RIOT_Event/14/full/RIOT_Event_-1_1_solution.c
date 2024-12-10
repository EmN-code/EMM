#include <stdio.h>
#include <event/periodic.h>
#include <event/callback.h>

event_periodic_t periodic_event;

void on_periodic_event(event_periodic_t *periodic_event, event_t post_event) {
    printf("Periodic event triggered\n");
}

int main(void) {
    // Initialize the periodic event structure
    event_periodic_init(&periodic_event);

    // Define the callback for the periodic event
    periodic_event.callback = on_periodic_event;
    periodic_event.event.arg = NULL;

    // Set and start the periodic event to fire every second (1000 ms)
    event_periodic_set(&periodic_event, EVENT_PRIO_MEDIUM, EVENT_PERIODIC_MS(1000), 0);

    // Process events (this would typically be in your main loop)
    while (1) {
        event_periodic_process(&periodic_event);
    }

    return 0;
}