#include <ztimer.h>
#include <stdio.h>

#define INTERVAL (1U * ZTIMER_MSEC_PER_SEC)  // 1 second interval

static ztimer_t timer;

// Callback function to be called every second
void timer_callback(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    printf("Timer callback function executed\n");
    ztimer_set(&timer, INTERVAL);
}

int main(void) {
    // Initialize the timer
    ztimer_set(&timer, INTERVAL);
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set first callback
    ztimer_set(&timer, INTERVAL);
    
    // Keeping the processor running to catch the timer callbacks
    while(1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
    return 0;
}