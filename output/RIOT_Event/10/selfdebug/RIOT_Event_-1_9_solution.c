#include <xtimer.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)

static xtimer_t timer;

// Callback function that will be called periodically
void timer_callback(void *arg) {
    (void)arg;
    printf("Timer event\n");
    
    // Reschedule the timer
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize timer with the callback function
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    // Set the timer
    xtimer_set(&timer, TIMER_INTERVAL);
    
    // Main loop does nothing, just wait for interrupts
    while(1) {
        // You could add more code here that should run continually
        xtimer_sleep(1);
    }
    
    return 0;
}