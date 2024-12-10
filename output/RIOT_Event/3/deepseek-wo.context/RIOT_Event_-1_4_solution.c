
#include <stdio.h>
#include "event.h"
#include "ztimer.h"
#include "thread.h"

#define EVENT_QUEUE_SIZE     (8)
#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIO    (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIO);

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback, NULL);

    event_periodic_callback_set_interval(&periodic_event, 1000000); // 1 second in microseconds
    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
