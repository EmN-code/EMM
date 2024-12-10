
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_t event_queue;
    char event_thread_stack[EVENT_THREAD_STACKSIZE];

    event_queue_init(&event_queue);
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), THREAD_PRIORITY_MAIN - 1);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, event_periodic_callback, NULL, 1 * US_PER_SEC);
    event_periodic_start(&periodic_event);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
