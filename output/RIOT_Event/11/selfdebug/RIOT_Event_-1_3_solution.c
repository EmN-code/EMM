#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * 1000000U) // Interval of 1 second in microseconds

void timer_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Timer callback triggered\n");
}

int main(void) {
    ztimer_periodic_t periodic_timer;

    // Initialize periodic timer with the callback function
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, timer_callback, NULL);
    
    // Start the periodic timer
    ztimer_periodic_start(&periodic_timer, TIMER_INTERVAL);

    // Run indefinitely, as callbacks will occur every second
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}