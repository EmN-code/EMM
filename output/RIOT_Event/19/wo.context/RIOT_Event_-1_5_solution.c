#include "ztimer.h"
#include "xtimer.h"
#include "ztimer_usec.h"
#include "event/periodic.h"

#define INTERVAL_SEC 1

void my_callback(void *arg) {
    (void)arg;
    printf("Callback invoked\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init(&evtimer, _evtimer_msg_handler);

    evtimer_event_t periodic_event;
    periodic_event.event.callback = my_callback;
    periodic_event.event.arg = NULL;

    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, &evtimer.timer, INTERVAL_SEC * (int64_t)ZTIMER_USEC_BASE, &periodic_event.event);
    ztimer_periodic_start(&periodic_timer);

    while (1) {
        ztimer_periodic_update(&periodic_timer);
        xtimer_sleep(INTERVAL_SEC);
    }

    return 0;
}