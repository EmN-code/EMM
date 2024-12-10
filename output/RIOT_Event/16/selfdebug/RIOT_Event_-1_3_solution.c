#include <ztimer.h>
#include <stdio.h>

#define INTERVAL (1U * ZTIMER_MSEC_PER_SEC) // 1 second interval

// Callback function for the timer
void timer_callback(void *arg) {
    (void)arg;
    puts("Timer callback executed.");
}

int main(void) {
    ztimer_t timer;
    
    // Initialize the ztimer with the callback function
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL, timer_callback, NULL);
    
    // Keep the program running to allow the timer to execute
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL); // Sleep to simulate periodic execution
    }
    
    return 0;
}