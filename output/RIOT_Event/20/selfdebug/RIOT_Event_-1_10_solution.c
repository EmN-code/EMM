#include "xtimer.h"
#include "event/callback.h"
#include <stdio.h>

#define TIMER_INTERVAL (1U * 1000000U)

void callback(void *arg) {
    (void)arg; // Suppressing unused parameter warning
    puts("Callback function executed");
}

int main() {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        callback(NULL);
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }
    
    return 0;
}