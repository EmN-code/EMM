#include <xtimer.h>
#include <stdio.h>
#include <event/periodic.h>

event_periodic_t periodic_event;

void callback(void *arg) {
    puts("Periodic event triggered!");
}

int main(void) {
    puts("Starting periodic event example...");

    // Initialize the periodic event
    event_periodic_init(&periodic_event, &event_queues[0], callback, NULL);
    
    // Set the periodic time to 1 second (1,000,000 microseconds)
    int period = 1000000;

    // Start the periodic event
    event_periodic_start(&periodic_event, period);

    // Enter the main loop
    while (1) {
        // Do other stuff here if needed
        xtimer_sleep(1);
    }

    return 0;
}