
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), 1);

    event_t periodic_event;
    periodic_event.handler = periodic_event_callback;
    periodic_event.arg = NULL;

    xtimer_t periodic_timer;
    periodic_timer.callback = periodic_event_callback;
    periodic_timer.arg = NULL;

    xtimer_set(&periodic_timer, 1 * US_PER_SEC);

    while (1) {
        event_t *event = event_wait(&event_queue);
        if (event) {
            event->handler(event->arg);
        }
    }

    return 0;
}
