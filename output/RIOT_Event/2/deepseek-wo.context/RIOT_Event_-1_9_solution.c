
#include "event.h"
#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

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
    event_queue_init(&event_queue);

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback);
    event_periodic_callback_set_count(&periodic_event, EVENT_PERIODIC_FOREVER);
    event_periodic_callback_set_interval(&periodic_event, 1000000); // 1 second in microseconds

    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
