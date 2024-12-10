#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * ZTIMER_MSEC)

static ztimer_t timer;

static void callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Callback function fired!\n");
    ztimer_set(&ztimer_msec, &timer, TIMER_INTERVAL);
}

int main(void) {
    puts("RIOT Timer Callback Example");
    
    // Initialize the timer
    timer.callback = callback;
    timer.interval = TIMER_INTERVAL;
    timer.arg = NULL;

    // Set the timer for the first time
    ztimer_set(&ztimer_msec, &timer, TIMER_INTERVAL);
    
    return 0;
}