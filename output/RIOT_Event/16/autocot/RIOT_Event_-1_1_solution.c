#include <event/periodic.h>
#include <event/callback.h>
#include <stdio.h>

static event_periodic_t event;

void periodic_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Periodic callback executed.\n");
}

int main(void) {
    // Initialize the periodic event
    event_periodic_init(&event, EVENT_PRIO_MEDIUM, periodic_callback, NULL);

    // Set the periodic time interval to 1 second (1000 milliseconds)
    event_periodic_set(&event, 1000 * US_PER_MS, EVENT_PRIO_MEDIUM);
    
    // Start the periodic event
    event_periodic_start(&event);

    // Let the main loop run indefinitely
    while (1) {
        // Could perform other tasks here or just idle
    }

    return 0;
}