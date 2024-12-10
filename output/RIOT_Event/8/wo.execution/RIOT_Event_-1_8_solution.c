#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "event/thread.h"
#include "event/periodic.h"

#define INTERVAL (1U * US_PER_SEC) // 1 second interval.

static void _periodic_callback(void *arg)
{
    (void)arg;
    puts("Event periodic callback triggered!");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_periodic_init(&evtimer, _periodic_callback, NULL,timer_callback,NULL);
    evtimer_start(&evtimer, INTERVAL);

    while (1) {
        evtimer_process(&evtimer);
    }

    return 0;
}