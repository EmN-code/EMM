#include <xtimer.h>
#include <event/periodic.h>
#include <event/callback.h>

#define INTERVAL (1U * US_PER_SEC)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();
    evtimer_t evtimer;
    evtimer_periodic_event_t event;

    evtimer_periodic_init(&event, INTERVAL, periodic_callback, NULL);
    evtimer_add_periodic(&evtimer, &event);

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        evtimer_dispatch(&evtimer);
    }

    return 0;
}