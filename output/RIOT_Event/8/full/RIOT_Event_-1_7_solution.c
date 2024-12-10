#include <xtimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ztimer.h>
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "net/sock/util.h"
#include "event/callback.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"

#define PERIOD (1U * US_PER_SEC)

static evtimer_t evtimer;
static evtimer_event_t event;

static void periodic_event(void)
{
    printf("Periodic event triggered\n");
    evtimer_add(&evtimer, &event.event);
}

int main(void)
{
    puts("RIOT event periodic example\n");
    evtimer_init(&evtimer, periodic_event);
    evtimer_add(&evtimer, &event.event);
    while (1) {
        evtimer_periodic_start(&evtimer, PERIOD);
        msg_t msg_queue[8];
        msg_init_queue(msg_queue, 8);
        evtimer_run();
    }
    return 0;
}