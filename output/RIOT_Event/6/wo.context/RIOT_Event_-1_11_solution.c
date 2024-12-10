#include <ztimer.h>
#include <stdio.h>
#include <event/callback.h>

#define TIMER_INTERVAL (1LU * 1000LU * 1000LU) // 1 second in microseconds

void event_periodic(void *arg) {
    (void)arg;  // Cast unused variable to void to avoid warnings
    printf("Event triggered\n");
}

int main(void) {
    ztimer_t timer;
    
    timer.callback = event_periodic;
    timer.arg = NULL;
    
    // Initialize the periodic timer
    ztimer_periodic_init(ZTIMER_MSEC, &timer);
    
    // Start the periodic timer with the defined interval
    ztimer_periodic_start(&timer, TIMER_INTERVAL);

    while (1) {
        // Sleep to let the timer run
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}