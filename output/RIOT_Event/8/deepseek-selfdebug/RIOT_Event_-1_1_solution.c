
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"

// Define the event timer structure
static evtimer_t periodic_timer;

// Callback function that will be called every second
void periodic_callback(void *arg) {
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init_periodic(&periodic_timer, periodic_callback, NULL);

    // Set the periodic event to trigger every second (1000000 microseconds)
    evtimer_add_periodic(&periodic_timer, 1000000);

    // Main loop to keep the program running
    while (1) {
        // Yield to allow other threads to run
        xtimer_sleep(1);
    }

    return 0;
}
