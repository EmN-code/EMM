#include "xtimer.h"
#include "event/timeout.h"
#include "time_units.h"

#define INTERVAL (1U * US_PER_SEC) // 1 second interval in microseconds

static void event_periodic_callback(void *arg) {
    (void)arg;
    // Code to be executed every second
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        // Schedule periodic event
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        event_periodic_callback(NULL);
    }

    return 0;
}