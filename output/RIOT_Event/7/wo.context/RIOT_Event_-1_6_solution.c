#include <ztimer.h>
#include <stdio.h>
#include <event.h>

#define PERIOD 1000000 // 1 second in microseconds

static void callback(void *arg) {
    (void)arg;
    printf("Callback triggered\n");
}

int main(void) {
    ztimer_t timer;
    evtimer_t evtimer;
    evtimer.event.callback = callback;
    evtimer.event.event_time = ztimer_now(ZTIMER_MSEC) + PERIOD;

    while (1) {
        ztimer_set(ZTIMER_MSEC, &timer, PERIOD);
        ztimer_sleep(ZTIMER_MSEC, PERIOD);
        evtimer_del(&evtimer, &evtimer.event);
        ztimer_handler();  // manage any pending events
    }
    return 0;
}