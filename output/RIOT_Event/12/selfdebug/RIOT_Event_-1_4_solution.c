#include <xtimer.h>
#include <stdio.h>

// Callback function to be called periodically
typedef struct {
    ztimer_t timer; // Timer structure
    uint32_t interval; // Time interval for periodic callback
} my_periodic_timer_t;

void my_timer_callback(void *arg) {
    (void)arg;
    puts("Periodic callback executed");
}

int main(void) {
    // Define periodic timer structure
    my_periodic_timer_t periodic_timer;
    periodic_timer.interval = 1 * US_PER_SEC; // Set interval to 1 second

    // Initialize the periodic timer
    ztimer_set(ZTIMER_SEC, &periodic_timer.timer, periodic_timer.interval, my_timer_callback, NULL);
    
    puts("Periodic timer initialized and started.");

    // The main loop continues running while timer operates in the background
    while (1) {
        xtimer_sleep(5); // Sleep to reduce CPU usage in this example
    }
    return 0;
}
