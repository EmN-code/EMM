
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;
    event_periodic_callback_t periodic_cb;

    periodic_cb.callback = periodic_callback;
    periodic_cb.arg = NULL;

    event_periodic_init(&periodic_event, &periodic_cb, XTIMER_TICKS(1000000));
    event_periodic_add_event(&periodic_event);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
