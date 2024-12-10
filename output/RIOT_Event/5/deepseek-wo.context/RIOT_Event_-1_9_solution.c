
#include <stdio.h>
#include "event/periodic.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static char stack[STACK_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t periodic_event;
    event_queue_t event_queue;

    event_thread_init(&event_queue, stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1);

    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback);
    event_periodic_callback_set_interval(&periodic_event, 1000000); // 1 second interval
    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
