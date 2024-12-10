#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "kernel_defines.h"
#include "thread.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "ztimer.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "mbox.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1U * US_PER_SEC)

static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;
static msg_t msg_queue[8];

static void _callback(void)
{
    puts("Periodic callback triggered");
}

int main(void)
{
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    evtimer_init_msg(&evtimer);

    evtimer_periodic_init(&evtimer_event, &evtimer, PERIODIC_INTERVAL, _callback, NULL);
    evtimer_add_msg(&evtimer, &evtimer_event, SCHED_PRIO_IDLE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}