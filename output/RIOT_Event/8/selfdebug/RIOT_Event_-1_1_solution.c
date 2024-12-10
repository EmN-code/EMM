#include <xtimer.h>
#include <stdio.h>
#include <event/periodic.h>

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_init_periodic(&evtimer);
    evtimer_add_periodic(&evtimer, &event, 1ULL * US_PER_SEC, periodic_callback, NULL);

    while (1) {
        evtimer_update_periodic(&evtimer);
        xtimer_sleep(1);
    }

    return 0;
}
