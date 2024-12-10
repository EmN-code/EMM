#include <ztimer.h>
#include <stdio.h>

void event_periodic_cb(void *arg) {
    printf("Periodic event triggered\n");
}

int main(void) {
    // Initialize and start the periodic event timer
    static ztimer_t timer;
    timer.callback = event_periodic_cb;
    timer.arg = NULL;
    ztimer_set_periodic(ZTIMER_MSEC, &timer, 1000);

    return 0;
}