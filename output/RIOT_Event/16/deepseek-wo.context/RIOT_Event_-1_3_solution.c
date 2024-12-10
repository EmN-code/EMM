
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"  // Include the necessary header for event handling

// Define the event callback function
void event_periodic_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event periodic timer
    event_periodic_t event_periodic;
    event_periodic_init(&event_periodic, event_periodic_callback, NULL);

    // Set the event periodic timer to trigger every second
    xtimer_periodic_wakeup(&event_periodic.timer, 1 * US_PER_SEC);

    // Main loop to keep the application running
    while (1) {
        // Wait for the next event
        event_wait(&event_periodic.event);
    }

    return 0;
}
