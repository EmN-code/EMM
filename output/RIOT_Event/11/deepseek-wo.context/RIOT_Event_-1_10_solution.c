
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

static char stack[STACK_SIZE];

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_queue_t queue;
    event_periodic_t periodic_event;

    event_thread_init(&queue, stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1);

    event_periodic_init(&periodic_event, xtimer_ticks32(xtimer_s2ticks(1)), periodic_callback, NULL);
    event_periodic_add(&periodic_event);

    while (1) {
        event_loop(&queue);
    }

    return 0;
}
