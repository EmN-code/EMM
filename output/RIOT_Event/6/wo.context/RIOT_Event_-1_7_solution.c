#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * ZTIMER_MSEC_PER_SEC)  // Timer duration of 1 second

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;  // Prevent unused variable warning
    puts("Timer callback fired");
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
}

int main(void) {
    puts("Initializing timer");
    
    // Initialize the timer
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
    timer.callback = timer_callback;
    timer.arg = NULL;

    return 0;
}