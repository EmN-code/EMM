
#include "xtimer.h"
#include "event/callback.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];

void periodic_callback(void *arg)
{
    (void)arg; // To avoid the unused parameter warning
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t queue;
    event_thread_init(&queue, stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1);

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)periodic_callback;
    xtimer_set(&timer, 1 * US_PER_SEC); // Trigger every second

    return 0;
}
