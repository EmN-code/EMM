
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

static char stack[STACKSIZE];

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    event_t event;

    event_queue_init(&event_queue);
    event.handler = event_periodic_callback;
    event.arg = NULL;

    xtimer_t timer;
    timer.callback = event_periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second interval

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
