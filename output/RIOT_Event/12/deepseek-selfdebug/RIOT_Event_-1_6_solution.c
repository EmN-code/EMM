
#include "xtimer.h"
#include "event/periodic.h"
#include "thread.h"
#include "event/callback.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks32(), xtimer_sec2ticks(1), periodic_callback, NULL);

    event_periodic_start(&periodic_event);

    return 0;
}
