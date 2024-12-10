
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

static void *event_thread(void *arg)
{
    (void)arg;
    while (1) {
        event_t event;
        event_wait(&event_queue, &event);
        if (event.handler) {
            event.handler(event.arg);
        }
    }
    return NULL;
}

int main(void)
{
    event_queue_init(&event_queue);
    thread_create(event_thread_stack, sizeof(event_thread_stack),
                  EVENT_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  event_thread, NULL, "event_thread");

    xtimer_t timer;
    timer_event_t timer_event;
    timer_event_init_periodic(&timer, &timer_event, event_periodic_callback, NULL, 1000000);

    while (1) {
        xtimer_set(&timer, 1000000);
        xtimer_usleep(1000000);
    }

    return 0;
}
