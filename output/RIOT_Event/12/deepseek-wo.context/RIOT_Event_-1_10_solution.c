
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
    event_thread_init(&event_queue, stack, STACKSIZE, PRIORITY);

    event_periodic_t periodic_event;
    periodic_event.event.handler = periodic_callback;
    periodic_event.event.arg = NULL;
    periodic_event.interval = 1 * US_PER_SEC;

    event_periodic_create(&event_queue, &periodic_event);

    return 0;
}
