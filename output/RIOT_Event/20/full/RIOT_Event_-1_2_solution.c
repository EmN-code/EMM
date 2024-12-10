#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define INTERVAL_SEC (1U)

static evtimer_t evtimer;
static evtimer_event_t event;

void event_callback(void *arg) {
    (void)arg;
    printf("Event triggered every second!\n");
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    evtimer_init_msg(&evtimer);

    event.callback = event_callback;
    event.period = ztimer_now(ZTIMER_MSEC) + (INTERVAL_SEC * MS_PER_SEC);
    evtimer_add_msg(&evtimer, &event, sched_active_pid);

    while (1) {
        msg_t m;
        msg_receive(&m);
    }

    return 0;
}