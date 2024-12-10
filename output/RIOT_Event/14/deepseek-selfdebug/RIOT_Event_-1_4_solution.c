
#include "xtimer.h"
#include "event/periodic.h"
#include "thread.h"

#define STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY     (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;

    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY);

    event_periodic_init(&periodic_event, &event_queue, xtimer_ticks_from_usec(1000000), periodic_callback, NULL);

    event_periodic_start(&periodic_event);

    return 0;
}
