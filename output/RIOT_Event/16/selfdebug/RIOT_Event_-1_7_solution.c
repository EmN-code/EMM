#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (ZTIMER_MSEC_TO_USEC(1000))  // 1000 milliseconds = 1 second

// Define a function to be called periodically
void callback(void *arg) {
    printf("Periodic callback executed.\n");
}

int main(void) {
    ztimer_periodic_t periodic_timer;

    // Initialize the periodic timer
    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, TIMER_INTERVAL, callback, NULL);

    // Let the timer run periodically
    ztimer_periodic_start(&periodic_timer);

    // Keep the main thread running
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);  // Sleep for 1 second each iteration
    }

    return 0;
}