#include <stdio.h>  // standard I/O operations
#include <event/periodic.h>  // for dealing with periodic events
#include <event/callback.h>  // to handle callbacks

// Define interval for periodic callback in microseconds (1 second)
#define INTERVAL_USEC      (1000000U)

// Function to be called periodically
void my_periodic_callback(void *arg) {
    (void)arg;  // avoid unused variable warning
    printf("Callback called!\n");
}

int main() {
    // Initialize periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event);

    // Set up periodic callback
    event_periodic_start_callback(&periodic_event, INTERVAL_USEC, my_periodic_callback, NULL);

    // Enter main loop
    while (1) {
        // Normally here it'd do more work or sleep
    }

    return 0;
}