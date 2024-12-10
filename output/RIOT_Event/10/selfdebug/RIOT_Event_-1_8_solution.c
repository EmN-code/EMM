#include <xtimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // Set the timer interval to 1 second

void timer_callback(void *arg) {
    puts("Timer callback triggered");
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now32();
    
    // Periodic event every second
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        timer_callback(NULL);
    }
    return 0;
}