#include "mma8x5x_regs.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mbox.h"
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "event/thread.h"
#include "netinet/in.h"
#include "event/callback.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define INTERVAL_US (1000000U)

static void _my_callback(void *arg)
{
    (void)arg;
    puts("Timer triggered!");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_event_t event;

    /* initialize the event timer for mbox events */
    evtimer_init_mbox(&evtimer);

    /* set event parameters */
    event.event.callback = _my_callback;
    event.event.period = INTERVAL_US;

    /* add the event to the timer queue */
    evtimer_add(&evtimer, &event.event);

    /* the main thread should do something useful here */
    while (1) {
        /* let the callback do the work */
        evtimer_run(&evtimer);
    }

    return 0;
}