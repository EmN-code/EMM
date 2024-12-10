#include "ztimer.h"

#include <stdio.h>

void event_periodic(void *arg) {
    (void)arg;
    printf("Timer event occurred\n");
    // Re-set the timer for one second
    ztimer_set(ZTIMER_SEC, &timer, 1);
}

int main(void) {
    printf("RIOT application starting\n");

    // Initialize and set the timer
    ztimer_t timer;
    ztimer_init(ZTIMER_SEC, &timer, event_periodic, NULL);
    ztimer_set(ZTIMER_SEC, &timer, 1);

    // Keep the main thread alive
    while(1) {
        // Busy-wait or put the CPU to sleep
    }
    return 0;
}