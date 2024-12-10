
#include <stdio.h>
#include <xtimer.h>
#include <event/periodic_callback.h>

// Define the callback function
void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Define the event_periodic_callback_t structure
    event_periodic_callback_t event;

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_USEC, US_PER_SEC, NULL, callback, NULL);

    // Start the periodic callback event
    event_periodic_callback_start(&event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
