
#include "xtimer.h"
#include "event/periodic_callback.h"
#include "thread.h"

#define STACK_SIZE (512)

static char stack[STACK_SIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t periodic_event;

    event_thread_init(&event_queue, stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1);

    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, 1000);
    event_periodic_callback_start(&periodic_event);

    return 0;
}
