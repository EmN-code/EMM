#include <xtimer.h>
#include <stdio.h>

#define INTERVAL_SEC 1

// Callback function to be executed every second
void callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Define the timer
    xtimer_t timer;
    
    // Initialize the timer
    xtimer_set_msg(&timer, INTERVAL_SEC * US_PER_SEC, NULL, 0);

    // Set the callback function
    xtimer_callback(&timer, callback, NULL);

    // Start the timer
    xtimer_set_periodic(&timer, INTERVAL_SEC * US_PER_SEC);

    // Loop to handle events
    while (1) {
        xtimer_spin(XTIMER_USEC(INTERVAL_SEC)); // Sleep until the next tick
    }

    return 0;
}