
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, periodic_callback, NULL, xtimer_ticks32(), xtimer_s2ticks(1));

    event_periodic_add(&periodic_event);

    event_thread_t event_thread;
    event_thread_create(&event_thread, stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, &event_queue, NULL);

    return 0;
}
