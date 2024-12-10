
#include <stdio.h>
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);

    event_t event;
    event.handler = (event_handler_t)periodic_callback;

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)event_queue_event_handler;
    timer.arg = &event;

    xtimer_set(&timer, XTIMER_TICKS(1000000)); // 1 second interval

    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
