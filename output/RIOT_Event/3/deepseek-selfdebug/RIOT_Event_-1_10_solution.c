
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic_callback.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, &ztimer_clock_sec, &event_queue, periodic_callback);
    event_periodic_callback_set_interval(&periodic_event, 1000000); // 1 second in microseconds
    event_periodic_callback_start(&periodic_event);

    return 0;
}
