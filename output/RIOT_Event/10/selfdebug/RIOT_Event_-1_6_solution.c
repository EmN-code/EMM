#include <ztimer.h>
#include <stdio.h>

#define INTERVAL 1000000 // Setting timer interval for 1 second in microseconds

void event_periodic(void *arg) {
    printf("Timer triggered!\n");
    // Re-initialize or reset any state if needed
    // Re-schedule the timer for periodic execution
    ztimer_set(ZTIMER_SEC, (ztimer_t *)arg, INTERVAL);
}

int main(void) {
    ztimer_t timer;
    timer.callback = event_periodic;
    timer.arg = &timer;

    ztimer_set(ZTIMER_SEC, &timer, INTERVAL);

    while (1) {
        // Main loop
    }

    return 0;
}