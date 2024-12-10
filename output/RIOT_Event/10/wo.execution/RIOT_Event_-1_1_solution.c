#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "event/callback.h"

// Callback function that will be called every second
event_callback_t periodic_callback;

// Function to be used as callback
void callback(void *arg) {
    (void)arg; // to suppress unused parameter warning
    printf("Callback executed\n");
}

// Main function
int main(void) {
    // Initialize the periodic event
    event_periodic_t periodic_event;
    periodic_callback.callback = callback;
    periodic_callback.arg = NULL;
    
    // Initialize timer and attach the periodic callback
    event_periodic_init(&periodic_event, &periodic_callback, SEC_IN_USEC);
    event_periodic_start(&periodic_event);

    // Let the main thread sleep in an endless loop
    while (1) {
        thread_sleep();
    }

    return 0;
}