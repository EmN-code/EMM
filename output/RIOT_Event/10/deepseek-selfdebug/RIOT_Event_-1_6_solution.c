
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    evtimer_periodic_t periodic_timer;
    evtimer_periodic_init(&periodic_timer, xtimer_ticks32(xtimer_s_to_usec(1)), periodic_callback, NULL);
    evtimer_periodic_add(&periodic_timer);

    while (1) {
        thread_yield();
    }

    return 0;
}
