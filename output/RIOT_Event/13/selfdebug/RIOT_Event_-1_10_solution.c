#include <ztimer.h>
#include <stdio.h>

#define INTERVAL ZTIMER_MSEC_BASE 1000U  // 1000 milliseconds = 1 second

void periodic_callback(void *arg) {
    printf("Periodic callback triggered\n");
}

int main(void) {
    ztimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    while (1) {
        ztimer_set(&timer, ZTIMER_MSEC_BASE, INTERVAL);
        ztimer_spin(ZTIMER_MSEC_BASE, INTERVAL); // Sleep for the interval
    }

    return 0;
}