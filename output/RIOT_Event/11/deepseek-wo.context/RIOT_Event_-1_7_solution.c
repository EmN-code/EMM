
#include "xtimer.h"
#include "event/periodic.h"
#include "event/thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_queue_t event_queue;
    char event_thread_stack[EVENT_THREAD_STACKSIZE];

    event_periodic_init(&periodic_event, xtimer_ticks64(xtimer_s2ticks(1)), periodic_callback, NULL);
    event_periodic_add(&periodic_event);

    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIO);

    return 0;
}
