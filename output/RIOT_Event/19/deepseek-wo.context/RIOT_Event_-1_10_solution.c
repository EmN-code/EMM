
#include <stdio.h>
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_cb(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, stack, STACKSIZE, PRIORITY);

    xtimer_ticks32_t interval = xtimer_ticks(xtimer_s2ticks(1));
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, interval, periodic_event_cb, NULL);
    event_periodic_add(&event_queue, &periodic_event);

    return 0;
}
