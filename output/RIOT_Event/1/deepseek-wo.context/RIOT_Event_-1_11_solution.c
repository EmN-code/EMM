
#include "xtimer.h"
#include "event.h"
#include "event/periodic.h"

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_periodic_callback_t periodic_event;

    event_queue_init(&event_queue);

    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback);
    event_periodic_callback_set_count(&periodic_event, EVENT_PERIODIC_FOREVER);
    event_periodic_callback_start(&periodic_event, 1 * US_PER_SEC);

    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
