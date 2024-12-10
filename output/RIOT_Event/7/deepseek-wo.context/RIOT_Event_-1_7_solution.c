
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_cb(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

static void event_loop(void)
{
    while (1) {
        event_t event;
        event_wait(&event_queue, &event);
        event.handler(&event);
    }
}

int main(void)
{
    event_queue_init(&event_queue);
    thread_create(event_thread_stack, sizeof(event_thread_stack),
                  EVENT_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)event_loop, NULL, "event_thread");

    xtimer_t timer;
    event_t periodic_event;
    periodic_event.handler = (event_handler_t)periodic_event_cb;
    periodic_event.arg = NULL;

    xtimer_init(&timer);
    while (1) {
        xtimer_set(&timer, 1000000); // 1 second
        xtimer_usleep(1000000); // 1 second
        event_post(&event_queue, &periodic_event);
    }

    return 0;
}
