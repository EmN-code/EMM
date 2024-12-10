
#include "thread.h"
#include "xtimer.h"
#include "event/callback.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void periodic_event_cb(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_queue_t event_queue;
    char event_thread_stack[EVENT_THREAD_STACKSIZE];

    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    evtimer_t periodic_timer;
    evtimer_init(&periodic_timer);

    evtimer_periodic_event_t periodic_event;
    periodic_event.event.handler = (event_handler_t)periodic_event_cb;
    periodic_event.event.arg = NULL;
    periodic_event.interval = (1 * US_PER_SEC);

    evtimer_add_periodic(&periodic_timer, &periodic_event);

    while (1) {
        thread_yield();
    }

    return 0;
}
