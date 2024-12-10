#include <ztimer.h>
#include <evtimer.h>
#include <stdio.h>
#include <xtimer.h>

#define US_PER_SEC 1000000U

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered.\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_event_t event;

    /* Initialize the event timer */
    evtimer_init(&evtimer, periodic_callback);

    /* Set up periodic event every second */
    evtimer_set(&evtimer, &event, US_PER_SEC);

    /* Dispatch the event timer */
    evtimer_dispatch(&evtimer);

    return 0;
}