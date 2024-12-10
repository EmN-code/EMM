
#include <stdio.h>
#include "xtimer.h"
#include "evtimer.h"  // Include the necessary header file

#define INTERVAL_MS (1000U)  // 1 second interval

static void event_periodic_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_t event_timer;
    evtimer_init(&event_timer);

    // Set up the periodic event
    evtimer_add_periodic(&event_timer, INTERVAL_MS * 1000, event_periodic_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        // The event timer will handle the periodic callback
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
