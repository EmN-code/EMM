#include "ztimer/ztimer.h"
#include "stdio.h"

#define INTERVAL (1U * ZTIMER_MSEC)  // Set the interval to 1 second (1000 milliseconds)

void event_periodic_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    printf("Callback triggered\n");
}

int main(void) {
    ztimer_t timer;
    ztimer_set_callback(&timer, event_periodic_callback, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
    return 0;
}