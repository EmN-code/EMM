
#include "xtimer.h"
#include "event/callback.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, event_thread_stack, EVENT_THREAD_STACKSIZE, EVENT_THREAD_PRIO);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks_from_usec(1000000), periodic_event_callback, NULL);

    event_periodic_add(&periodic_event);

    return 0;
}
