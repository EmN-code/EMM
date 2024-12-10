
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;

    event_periodic_callback_t callback = {
        .callback = periodic_callback,
        .arg = NULL
    };

    xtimer_ticks32_t ticks = xtimer_ticks32(xtimer_s_to_usec(INTERVAL_SEC));

    event_periodic_init(&periodic_event, ticks, &callback);
    event_periodic_add(&periodic_event);

    return 0;
}
