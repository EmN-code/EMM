#include <ztimer.h>
#include <stdio.h>

// ztimer_periodic structure initialization
ztimer_periodic_t timer;

// Callback function to be called by the timer
void callback(void *arg) {
    puts("Callback triggered every second!");
}

int main(void) {
    // Initialize the ztimer_periodic with the ztimer clock and callback function
    ztimer_periodic_init(ZTIMER_MSEC, &timer, 1000, callback, NULL);
    
    // Start the periodic timer
    ztimer_periodic_start(&timer);

    // Keep the program running
    while (1) {
        // The main loop can perform other tasks
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}