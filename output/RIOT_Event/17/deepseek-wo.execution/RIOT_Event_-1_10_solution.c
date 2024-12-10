
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>

// Define the callback function
void event_periodic_callback(void) {
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    // Initialize the timer
    xtimer_t timer;

    // Set the timer to trigger the callback every second
    while (1) {
        xtimer_set(&timer, 1000000); // 1 second in microseconds
        xtimer_usleep(1000000); // Sleep for 1 second
        event_periodic_callback();
    }

    return 0;
}
