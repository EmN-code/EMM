
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>

// Define a callback function that will be called every second
void event_periodic_callback(void) {
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    // Initialize the xtimer
    xtimer_init();

    // Set up a periodic timer to call the callback function every second
    xtimer_t timer;
    xtimer_periodic_wakeup(&timer, 1000000); // 1 second in microseconds

    while (1) {
        // Wait for the timer to expire
        xtimer_periodic_wakeup(&timer, 1000000);

        // Call the periodic callback function
        event_periodic_callback();
    }

    return 0;
}
