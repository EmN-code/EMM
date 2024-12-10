#include <ztimer.h>
#include <stdio.h>

typedef struct {
    ztimer_t timer;
    // Additional fields can be added here if needed
} evtimer_t;

void callback(void *arg) {
    (void)arg;
    printf("Timer event triggered.\n");
}

int main(void) {
    evtimer_t evtimer;
    ztimer_periodic_t periodic_event;

    // Initialize the timer
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_event, callback, NULL, 1000);
    ztimer_periodic_start(&periodic_event);

    while (1) {
        // Dispatch any events (if required by the overall system design)
        // evtimer_dispatch(&evtimer);
        ztimer_sleep(ZTIMER_MSEC, 1000); // sleep for 1 second
    }

    return 0;
}