
#include <stdio.h>
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t queue;
    char stack[STACKSIZE];

    event_queue_init(&queue);
    event_thread_init(&queue, stack, sizeof(stack), PRIORITY);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, periodic_callback, NULL, xtimer_ticks32(), xtimer_s_to_usec(1));
    event_add_periodic(&periodic_event);

    return 0;
}
