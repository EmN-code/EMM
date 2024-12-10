
#include <stdio.h>
#include "event/periodic.h"
#include "ztimer.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t periodic_event;
    ztimer_clock_t *clock = ZTIMER_USEC;
    event_queue_t *queue = &event_queue_default;

    event_periodic_callback_init(&periodic_event, clock, queue, periodic_callback);
    event_periodic_callback_set_interval(&periodic_event, PERIODIC_INTERVAL);
    event_periodic_callback_start(&periodic_event);

    return 0;
}
