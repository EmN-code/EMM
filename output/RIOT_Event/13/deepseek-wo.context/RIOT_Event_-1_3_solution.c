
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);

    event_t periodic_event;
    event_periodic_t periodic_event_source;

    event_periodic_init(&periodic_event_source, &periodic_event, periodic_event_callback, NULL, 1000000);
    event_periodic_start(&periodic_event_source);

    thread_create(event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIO, THREAD_CREATE_STACKTEST, event_thread, &event_queue, "event_thread");

    return 0;
}
