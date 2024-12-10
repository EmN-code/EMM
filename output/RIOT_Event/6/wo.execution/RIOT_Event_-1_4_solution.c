#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"

#define INTERVAL_SEC (1U)  // 1-second interval

static void callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    evtimer_t evtimer;
    evtimer_msg_event_t periodic_event;

    evtimer_init_msg(&evtimer);

    periodic_event.event.period.sec = INTERVAL_SEC;
    periodic_event.event.period.usec = 0;
    periodic_event.msg.type = 0;
    periodic_event.msg.content.ptr = NULL;

    evtimer_add(&evtimer, &periodic_event.event);

    while (1) {
        msg_receive(&periodic_event.msg);
        callback(NULL);
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_SEC * 1000);
    }

    return 0;
}