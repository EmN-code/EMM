
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    char event_thread_stack[EVENT_THREAD_STACKSIZE];

    event_queue_init(&event_queue);
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), THREAD_PRIORITY_MAIN - 1);

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)event_periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second interval

    return 0;
}
