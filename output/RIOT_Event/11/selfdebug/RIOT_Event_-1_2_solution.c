#include <ztimer.h>
#include <stdio.h>

// Define the periodic timer variable
ztimer_periodic_t periodic_timer;

// Callback function that gets called by the timer
static void timer_callback(void *arg) {
    (void)arg;  // Suppress unused variable warning
    puts("Periodic timer callback called");
    // Additional task can be performed here.
}

// Main function
int main(void) {
    // Initialize the periodic timer with a 1-second interval
    ztimer_periodic_init(&periodic_timer, ZTIMER_SEC, timer_callback, NULL);

    // Start the periodic timer
    ztimer_periodic_start(&periodic_timer, 1);

    // Keep the program running to let the timer function work
    while(1) {
        // Could add other tasks here in the future
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}