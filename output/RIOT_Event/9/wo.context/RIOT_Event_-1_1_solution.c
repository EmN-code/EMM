#include <xtimer.h>
#include <stdio.h>

// Timer variable
static xtimer_t timer;

// This is the callback function that will be called periodically
void timer_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Periodic callback called!\n");

    // Re-arm the timer for the next callback after 1 second (1e6 microseconds)
    xtimer_set(&timer, 1000000);
}

int main(void) {
    printf("RIOT Timer Example - Periodic Callback\n");

    // Initialize timer variable
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer for the first callback after 1 second
    xtimer_set(&timer, 1000000);

    // Enter the main event loop
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second
    }

    return 0;
}