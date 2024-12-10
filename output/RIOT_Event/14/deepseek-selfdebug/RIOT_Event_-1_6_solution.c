
#include "xtimer.h"
#include "thread.h"
#include "event.h"
#include "event/periodic.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), THREAD_PRIORITY_MAIN - 1);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks_from_usec(1000000), periodic_callback, NULL);
    event_periodic_start(&periodic_event);

    return 0;
}
