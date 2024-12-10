#include <event/periodic.h>
#include <event/callback.h>
#include <stdio.h>
#include <stdbool.h>

void callback(void *arg) {
    puts("Event periodic callback triggered.");
}

int main(void) {
    evtimer_event_t event;
    evtimer_t evtimer;

    // Initialize the evtimer
    evtimer_init_msg(&evtimer);

    // Set the callback function
    event.event.fn = callback;

    // Initialize the periodic event (1-second interval)
    evtimer_set_periodic(&evtimer, &event, 1);

    // Start the event
    evtimer_pending(&evtimer, true);

    // Simulation loop to keep the program running
    while (true) {
        // In a real scenario, this would likely be part of a main loop handling events.
    }

    return 0;
}