#include <ztimer.h>
#include <stdio.h>
#include <event/event.h>

// Callback function that gets executed periodically
void periodic_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Callback triggered!\n");
}

int main(void) {
    // Initialize a ztimer instance
    ztimer_t timer;
    
    // Define the interval (e.g., 1 second in ticks)
    uint32_t interval = 1000000;
    
    // Initialize the timer with the callback function
    timer.callback = periodic_callback;
    timer.arg = NULL;

    // Create the event
    evtimer_event_t event;
    event.event.period = interval;
    
    // Define the event timer
    evtimer_t evtimer;
    evtimer.timer = timer;
    evtimer.callback = periodic_callback;
    evtimer.events = &event;

    // Start the periodic callback every second
    evtimer_add_msg(&evtimer, (evtimer_msg_event_t *)&event, 0);

    while (1) {
        // Main loop can perform other tasks
    }

    return 0;
}