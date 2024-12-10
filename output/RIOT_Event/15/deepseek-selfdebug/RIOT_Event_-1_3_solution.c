
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "event/periodic.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_periodic_t periodic_event;

    event_queue_init(&event_queue);
    event_periodic_init(&periodic_event, xtimer_ticks32(xtimer_s_to_ticks(1)), periodic_callback, NULL);
    event_periodic_start(&periodic_event, &event_queue);

    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
