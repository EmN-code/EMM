#include <stdio.h>
#include <evtimer.h>
#include <event.h>
#include <xtimer.h>
#include <event/callback.h>

// Dummy callback function to be invoked by the event timer
static void callback(void *arg) {
    (void)arg; // Avoid unused variable warning
    printf("Callback function executed every second.\n");
}

int main(void) {
    // Define the event timer
    evtimer_t evtimer;
    evtimer_event_t event;
    
    // Initialize the event timer with a callback handler
    evtimer_init(&evtimer, event_callback_handler);
    
    // Initialize the event with timeout set to 1 second (1000000 us)
    evtimer_set(&event, 1000000); // 1000000 microseconds = 1 second
    event.context = NULL;
    event.callback = callback;
    
    // Add periodic event to be triggered every second
    evtimer_add_periodic(&evtimer, &event);
    
    while (1) {
        // Handle events
        evtimer_run(&evtimer);
        xtimer_sleep(1);
    }

    return 0;
}