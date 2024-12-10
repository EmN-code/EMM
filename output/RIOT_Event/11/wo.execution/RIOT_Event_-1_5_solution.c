#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "event/callback.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

void event_periodic(void *arg) {
    (void)arg;
    printf("Event triggered every second\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);
    event.event.callback = event_periodic;
    event.event.argument = NULL;
    event.event.period = ztimer_usec_from_sec(1);
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        thread_yield();
    }

    return 0;
}