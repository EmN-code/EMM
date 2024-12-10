#include <stdio.h>
#include "xtimer.h"

// Callback function that will be called every second
void event_periodic(void *arg) {
    (void)arg;  // avoid unused parameter warning
    puts("Event triggered: 1 second has passed.");
}

int main(void) {
    puts("RIOT ESP32 periodic event example.");
    
    // Periodic timer setup
    const xtimer_ticks32_t interval = xtimer_ticks_from_ms(1000);
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Infinite loop to call the callback function every second
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, interval);
        event_periodic(NULL);
    }

    return 0;
}