#include <stdio.h>
#include <event/periodic.h>
#include <event/callback.h>

void callback(void *arg) {
    printf("Periodic event triggered!\n");
}

int main() {
    evtimer_periodic_t periodic_event;
    
    event_callback_t cb;
    
    // Configure the callback structure
    event_callback_init(&cb, callback, NULL);

    // Initialize the periodic event
    evtimer_periodic_init(&periodic_event);

    // Set the periodic event to trigger every second (1000 milliseconds)
    evtimer_set_periodic(&periodic_event.event, 1000U);

    // Add the callback to the event
    evtimer_add_event_cb(&periodic_event, &cb.super);

    // Define a loop to keep the program running
    while (1) {
        // Normally RIOT would keep the main thread running to handle events
    }

    return 0;
}