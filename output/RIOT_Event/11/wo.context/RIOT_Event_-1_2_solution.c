#include <ztimer.h>  // for ztimer and related functions
#include <stdio.h>   // for printf function

#define INTERVAL (1U * US_PER_SEC) // Interval time is defined in microseconds per second.

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer callback triggered!\n");
}

int main(void) {
    ztimer_periodic_t timer;
    ztimer_periodic_init(ZTIMER_USEC, &timer, timer_callback, NULL, INTERVAL);
    ztimer_periodic_start(&timer);

    while (1) {
        ztimer_periodic_dispatch(&timer);
    }
    return 0;
}