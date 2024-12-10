#include <xtimer.h>
#include <stdio.h>
#include "event/periodic.h"

#define PERIOD 1U

void periodic_event_callback(void *arg)
{
    (void)arg;
    puts("Periodic callback triggered.");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_event_t event;

    printf("Setting up periodic event every %u second(s)\n", PERIOD);

    evtimer_init(&evtimer);
    evtimer_set_periodic(&evtimer, &event, PERIOD);
    evtimer_event_callback_set(&event, periodic_event_callback, NULL);

    while (1) {
        evtimer_periodic_sleep(&evtimer);
    }

    return 0;
}