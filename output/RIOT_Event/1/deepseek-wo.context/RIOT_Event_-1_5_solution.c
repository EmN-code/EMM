
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, stack, STACK_SIZE, PRIORITY);

    event_periodic_callback_t periodic_event;
    event_periodic_callback_create(&periodic_event, ZTIMER_USEC, 1000000, &event_queue, periodic_callback, NULL);

    return 0;
}
