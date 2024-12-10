
#include "xtimer.h"
#include "thread.h"
#include "event.h"
#include "event_thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

static void event_periodic(void *arg)
{
    (void)arg;
    xtimer_periodic_wakeup((xtimer_ticks32_t *)arg, XTIMER_TICKS(1000000));
    event_post(&event_queue, (event_t *)arg);
}

int main(void)
{
    event_queue_init(&event_queue);
    event_thread_init(&event_queue, stack, STACKSIZE, PRIORITY);

    event_t event;
    event.handler = periodic_callback;

    thread_create(stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST, event_periodic, &event, "periodic_event");

    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
