
#include "xtimer.h"
#include "event/periodic_callback.h"
#include "event/callback.h"

// Define the periodic callback function
void periodic_callback(void *arg)
{
    (void)arg; // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_USEC, NULL, periodic_callback);

    // Start the periodic callback with a 1-second interval
    event_periodic_callback_start(&event, 1000000); // 1 second in microseconds

    // Main loop to keep the application running
    while (1) {
        // Process events (this is necessary to handle the periodic callback)
        event_loop(NULL);
    }

    return 0;
}
