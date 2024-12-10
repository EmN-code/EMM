
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "event/thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks32(), xtimer_s2ticks(1), periodic_callback, NULL);

    event_periodic_add(&periodic_event);

    event_thread_start(&event_queue);

    return 0;
}
