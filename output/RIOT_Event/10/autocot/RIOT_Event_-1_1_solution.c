#include <timex.h>
#include <thread.h>
#include <event/periodic.h>
#include <stdio.h>

static void callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Callback triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;
    timex_t interval = timex_set(1, 0);  // 1 second interval

    printf("Initializing event timer...\n");
    evtimer_init(&evtimer, _evtimer_msg_handler);

    printf("Adding periodic event...\n");
    evtimer_add_periodic(&evtimer, &event, interval, callback, NULL);

    printf("Event loop starting...\n");
    while(1) {
        evtimer_periodic_start(&evtimer);
        xtimer_sleep(interval.seconds);
    }
    return 0;
}