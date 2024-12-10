
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static event_queue_t event_queue;
static char event_thread_stack[EVENT_THREAD_STACKSIZE];

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

static void event_thread(void *arg)
{
    (void)arg;
    while (1) {
        event_process(&event_queue);
    }
}

int main(void)
{
    event_queue_init(&event_queue);

    thread_create(event_thread_stack, sizeof(event_thread_stack),
                  EVENT_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  event_thread, NULL, "event_thread");

    xtimer_t timer;
    timer.callback = periodic_event_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second

    return 0;
}
