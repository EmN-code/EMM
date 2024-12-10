#include "ztimer.h"
#include "ztimer_periph.h"
#include "stdio.h"

#define TIMER_INTERVAL (1U * ZTIMER_SEC) // Timer interval of 1 second

static ztimer_periodic_t periodic_timer;

// Callback function to be called every second
void callback(void *arg) {
    puts("Periodic callback triggered!");
    (void)arg;
}

int main(void) {
    // Initialize the periodic timer with the callback function
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, callback, NULL, TIMER_INTERVAL);
    
    // Keep the application running
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL); // Sleep for the interval duration
    }
    return 0;
}
