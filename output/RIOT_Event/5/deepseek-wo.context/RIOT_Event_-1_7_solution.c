
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "thread.h"
#include "xtimer.h"

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
    event_periodic_callback_t periodic_event;

    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, 1000);
    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
