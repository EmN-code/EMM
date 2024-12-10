#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/periodic.h"

#include <stdio.h>

void my_event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    evtimer_t evtimer;

    evtimer_init_msg(&evtimer);

    /* create a periodic event that triggers every second (1,000,000 us) */
    event_periodic_t periodic_event;
    period_usec = 1000000;
    event_periodic_init(&periodic_event, &evtimer.super, period_usec, my_event_handler, NULL);

    event_periodic_start(&periodic_event);

    /* The event loop that continuously checks and executes events */
    event_run();

    return 0;
}