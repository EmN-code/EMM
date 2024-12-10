#include <xtimer.h>
#include <stdio.h>
#include <event/periodic.h>
#include <event/callback.h>

static event_periodic_t periodic_event;

void callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void) {
    printf("Starting periodic event example\n");

    // Initialize the period event structure
    event_periodic_init(&periodic_event);

    // Set the period to 1 second, and the callback to our function
    event_periodic_set(&periodic_event, 1 * US_PER_SEC, callback, NULL);

    // Main loop - process events
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}