
#include "event.h"
#include "ztimer.h"
#include "thread.h"
#include "xtimer.h"

#define EVENT_QUEUE_SIZE (8)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char _event_thread_stack[STACK_SIZE];
static event_queue_t _event_queue;

static void _periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t periodic_event;

    event_thread_init(&_event_queue, _event_thread_stack, STACK_SIZE, 1);

    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &_event_queue, _periodic_callback, NULL);
    event_periodic_callback_start(&periodic_event, 1000);

    while (1) {
        event_process(&_event_queue);
    }

    return 0;
}
