
#include "xtimer.h"
#include "event/periodic.h"

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    event_periodic_t periodic_event;

    // Initialize the periodic event
    event_periodic_init(&periodic_event, &timer, periodic_callback, NULL);

    // Set the timer to trigger every second
    xtimer_set(&timer, 1000000);

    // Main loop to keep the application running
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
