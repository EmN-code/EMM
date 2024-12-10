
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    xtimer_ticks32_t interval = xtimer_ticks32(1 * US_PER_SEC);
    event_periodic_callback_init(&event_queue, interval, periodic_callback, NULL);
    event_periodic_callback_start(&event_queue);

    return 0;
}
