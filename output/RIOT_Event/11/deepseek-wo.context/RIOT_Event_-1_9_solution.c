
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static char stack[STACK_SIZE];

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_periodic_t periodic_event;

    event_queue_init(&event_queue);
    event_periodic_init(&periodic_event, &event_queue, periodic_event_callback, NULL);
    event_periodic_add(&periodic_event, xtimer_ticks32(1 * US_PER_SEC));

    while (1) {
        event_wait(&event_queue);
    }

    return 0;
}
