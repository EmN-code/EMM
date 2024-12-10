
#include "ztimer.h"
#include "event.h"
#include "thread.h"
#include "xtimer.h"

#define INTERVAL_SEC (1U)

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
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_SEC * MS_PER_SEC);
    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
