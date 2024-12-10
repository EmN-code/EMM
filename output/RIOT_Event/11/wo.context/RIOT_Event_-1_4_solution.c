#include <ztimer.h>
#include <stdio.h>

void timer_callback(void *arg) {
    puts("Timer callback triggered.");
}

int main(void) {
    ztimer_periodic_t timer;
    uint32_t interval = 1000000; // 1 second in microseconds
    ztimer_periodic_init(ZTIMER_MSEC, &timer, timer_callback, NULL, interval);
    
    while (1) {
        ztimer_periodic_wait(&timer);
    }
    
    return 0;
}