
#include "xtimer.h"
#include "event/periodic.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void event_callback(void *arg)
{
    (void)arg;
    printf("Event callback triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;

    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY);

    xtimer_ticks32_t interval = xtimer_ticks(xtimer_s_to_ticks(1));
    event_periodic_init(&periodic_event, interval, event_callback, NULL);
    event_periodic_start(&periodic_event, &event_queue);

    return 0;
}
