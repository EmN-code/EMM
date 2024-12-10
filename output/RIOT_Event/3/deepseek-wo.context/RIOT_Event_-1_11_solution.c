
#include <stdio.h>
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];
static event_queue_t event_queue;

static void callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t periodic_event;

    event_thread_init(&event_queue, stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1);

    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, callback, NULL);
    event_periodic_callback_start(&periodic_event, 1000);

    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
