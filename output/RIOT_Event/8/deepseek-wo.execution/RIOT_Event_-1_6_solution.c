
#include <stdio.h>
#include <xtimer.h>

// Define a callback function for the periodic event
void periodic_callback(void) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the xtimer
    xtimer_init();

    // Set a periodic timer to trigger every second
    xtimer_t timer;
    uint32_t period = 1000000; // 1 second in microseconds

    // Initialize the timer
    xtimer_set(&timer, period, (void (*)(void *))periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Sleep for a short period to avoid busy-waiting
        xtimer_usleep(100000); // 100 milliseconds
    }

    return 0;
}
