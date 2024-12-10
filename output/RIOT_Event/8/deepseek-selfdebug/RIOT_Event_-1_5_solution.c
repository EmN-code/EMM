
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void event_periodic_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    printf("Event periodic callback triggered\n");
}

int main(void) {
    xtimer_t timer;
    event_t event;

    // Initialize the event structure
    event.handler = (event_handler_t)event_periodic_callback;
    event.arg = NULL;

    // Set the timer to trigger the event every second
    xtimer_set_event_periodic(&timer, TIMER_INTERVAL, &event);

    // Main loop to keep the application running
    while (1) {
        // Process events
        xtimer_usleep(10000);  // Sleep for 10 ms to avoid busy-waiting
    }

    return 0;
}
