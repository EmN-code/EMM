
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    periodic_event_t periodic_event;
    periodic_event_init(&periodic_event, periodic_event_callback, NULL);

    xtimer_periodic_event_init_msg(&periodic_event, &event_queue, 1 * US_PER_SEC);

    periodic_event_start(&periodic_event);

    return 0;
}
