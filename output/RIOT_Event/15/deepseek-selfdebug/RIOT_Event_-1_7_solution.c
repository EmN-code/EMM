
#include <stdio.h>
#include "event.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    char stack[STACKSIZE];

    event_queue_init(&event_queue);
    event_thread_init(&event_queue, stack, STACKSIZE, PRIORITY);

    xtimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second interval

    return 0;
}
