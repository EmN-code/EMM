#include <ztimer.h>
#include <stdio.h>
#include <evtimer.h>

#define INTERVAL 1U // 1 second interval

static evtimer_t evtimer;
static evtimer_msg_event_t event;
static ztimer_t timer;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Event triggered every second\n");
    // Re-arm the timer for the next callback
    ztimer_set(ZTIMER_USEC, &timer, INTERVAL * US_PER_SEC);
}

int main(void) {
    puts("RIOT Event Timer Example");

    // Initialize the evtimer
    evtimer_init(&evtimer);

    // Initialize and set up periodic callback timer
    ztimer_init();
    timer.callback = periodic_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_USEC, &timer, INTERVAL * US_PER_SEC);

    // Start event dispatch loop
    while(1) {
        evtimer_dispatch(&evtimer);
        // Sleep to allow the system to idle
        ztimer_sleep(ZTIMER_MSEC, 100);
    }

    return 0;
}