
#include "event/periodic.h"
#include "event/callback.h"
#include "ztimer.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t queue;
static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_init(&queue);
    event_periodic_callback_create(&periodic_event, ZTIMER_USEC, 1000000, &queue, periodic_callback);
    event_periodic_callback_set_interval(&periodic_event, 1000000);
    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_process(&queue);
    }

    return 0;
}
