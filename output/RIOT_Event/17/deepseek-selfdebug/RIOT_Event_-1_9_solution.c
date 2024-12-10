
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_t event;
static xtimer_t timer;

void event_periodic_callback(void *arg)
{
    (void)arg;
    puts("Event periodic callback triggered");
}

int main(void)
{
    event_queue_t queue;
    event_queue_init(&queue);

    event_periodic_init(&timer, XTIMER_TICKS(1000000), event_periodic_callback, NULL);
    event_periodic_start(&timer);

    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, event_thread, &queue, "event_thread");

    return 0;
}
