
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void event_callback(event_t *event)
{
    (void)event;
    printf("Event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_t event;

    event_queue_init(&event_queue);
    event.handler = event_callback;

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, &event_queue, &event, 1000000U);

    event_periodic_add(&periodic_event);

    while (1) {
        event_t *ev = event_wait(&event_queue);
        if (ev) {
            ev->handler(ev);
        }
    }

    return 0;
}
