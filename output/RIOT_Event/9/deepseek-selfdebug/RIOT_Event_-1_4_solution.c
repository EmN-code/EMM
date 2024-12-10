
#include "xtimer.h"
#include "thread.h"
#include "event.h"

#define STACKSIZE 512

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    xtimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second interval

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
