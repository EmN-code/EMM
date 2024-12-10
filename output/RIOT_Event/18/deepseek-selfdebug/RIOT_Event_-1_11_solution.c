
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

static void *event_thread_func(void *arg)
{
    (void)arg;
    event_queue_init(&event_queue);

    while (1) {
        event_t event;
        event_wait(&event_queue, &event);
        event.handler(&event);
    }

    return NULL;
}

int main(void)
{
    event_t periodic_event;
    periodic_event.handler = periodic_event_callback;
    periodic_event.arg = NULL;

    thread_create(event_thread_stack, sizeof(event_thread_stack),
                  EVENT_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  event_thread_func, NULL, "event_thread");

    xtimer_t periodic_timer;
    periodic_timer.callback = (xtimer_callback_t)event_post;
    periodic_timer.arg = &periodic_event;

    xtimer_set(&periodic_timer, 1 * US_PER_SEC);

    return 0;
}
