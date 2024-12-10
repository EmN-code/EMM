
#include "xtimer.h"
#include "event/periodic.h"
#include "event/thread.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_periodic_t periodic_event;
    char event_thread_stack[EVENT_THREAD_STACKSIZE];

    /* Initialize the event queue */
    event_queue_init(&event_queue);

    /* Initialize the periodic event */
    event_periodic_init(&periodic_event, xtimer_ticks32(), xtimer_s2ticks(1), periodic_callback, NULL);

    /* Add the periodic event to the event queue */
    event_periodic_add(&event_queue, &periodic_event);

    /* Initialize the event thread */
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    /* Start the event thread */
    event_thread_start();

    return 0;
}
