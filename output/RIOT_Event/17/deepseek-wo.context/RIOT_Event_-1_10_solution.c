
#include "xtimer.h"
#include "event.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    char stack[EVENT_THREAD_STACKSIZE];

    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY_MAIN - 1);

    xtimer_t timer;
    timer.callback = periodic_event_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second interval

    return 0;
}
