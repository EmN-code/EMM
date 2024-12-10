#include "xtimer.h"
#include "ztimer.h"
#include "ztimer_periph.h"

// Define the interval for the timer in microseconds
#define TIMER_INTERVAL (1U * US_PER_SEC)

// Timer callback function
void timer_callback(void *arg) {
    (void)arg;
    puts("Timer tick");
}

// Main function
int main(void) {
    ztimer_t timer;
    
    // Initialize the timer
    ztimer_init(ZTIMER_PERIPH_TIMER0, &timer, timer_callback, NULL);

    // Start the periodic timer
    ztimer_set(ZTIMER_PERIPH_TIMER0, &timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Let the timer handle its events
        ztimer_periodic_wakeup(ZTIMER_PERIPH_TIMER0, TIMER_INTERVAL);
    }

    return 0;
}