
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_cb(void *arg)
{
    (void)arg;
    printf("Periodic event callback triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    xtimer_t timer;
    timer.callback = periodic_event_cb;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second interval

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
