
#include "ztimer.h"
#include "event.h"
#include "thread.h"
#include "xtimer.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    ztimer_periodic_callback_t periodic_event;
    ztimer_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    ztimer_periodic_callback_set_interval(&periodic_event, INTERVAL_MS);
    ztimer_periodic_callback_start(&periodic_event);

    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
