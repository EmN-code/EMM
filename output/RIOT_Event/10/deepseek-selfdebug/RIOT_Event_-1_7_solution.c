
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define PERIODIC_INTERVAL_SEC (1U)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;
    event_periodic_callback_t periodic_cb;

    periodic_cb.callback = periodic_callback;
    periodic_cb.arg = NULL;

    event_periodic_init(&periodic_event, &periodic_cb, xtimer_ticks32(), xtimer_s_to_usec(PERIODIC_INTERVAL_SEC));
    event_periodic_add(&periodic_event);

    while (1) {
        xtimer_sleep(PERIODIC_INTERVAL_SEC);
    }

    return 0;
}
