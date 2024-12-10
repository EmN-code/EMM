#define ZTIMER_MSEC_BASE ZTIMER_BASE

#include <ztimer.h>
#include <stdio.h>

static void callback(void *arg) {
    (void)arg;
    printf("Timer callback executed\n");
}

int main(void) {
    ztimer_t periodic_timer;
    uint32_t interval = 1000; // interval in milliseconds

    ztimer_init(ZTIMER_MSEC_BASE);

    ztimer_set(ZTIMER_MSEC_BASE, &periodic_timer, interval, callback, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC_BASE, interval);
    }

    return 0;
}