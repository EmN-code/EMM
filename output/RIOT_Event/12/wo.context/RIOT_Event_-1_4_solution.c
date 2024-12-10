#include <xtimer.h>
#include <stdio.h>
#include <evtimer.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

void callback(void *arg) {
    (void)arg;
    printf("Callback triggered every second!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_init(&evtimer);

    event.callback = callback;  // Correctly assign the callback function
    event.arg = NULL;            // No argument for the callback
    event.period = INTERVAL;     // Set the period for the event

    evtimer_add(&evtimer, &event);

    while (1) {
        evtimer_poll(&evtimer);
        xtimer_sleep(1);  // Sleep for one second (ensures callback is periodically triggered)
    }

    return 0;
}