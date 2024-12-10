
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "event/thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    char event_thread_stack[EVENT_THREAD_STACKSIZE];

    event_queue_init(&event_queue);
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    xtimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second interval

    return 0;
}
