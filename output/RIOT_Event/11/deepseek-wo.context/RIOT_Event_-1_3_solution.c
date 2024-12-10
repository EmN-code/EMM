
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_cb(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIO);

    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second interval
    event_periodic_t periodic_event;

    event_periodic_init(&periodic_event, interval, periodic_event_cb, NULL);
    event_periodic_start(&periodic_event);

    return 0;
}
