
#include "xtimer.h"
#include "event.h"
#include "ztimer.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t periodic_event;
    event_queue_t event_queue;

    event_queue_init(&event_queue);

    event_periodic_callback_create(&periodic_event, ZTIMER_USEC, INTERVAL_SEC * US_PER_SEC, &event_queue, periodic_callback);

    event_periodic_callback_start(&periodic_event, INTERVAL_SEC * US_PER_SEC);

    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
