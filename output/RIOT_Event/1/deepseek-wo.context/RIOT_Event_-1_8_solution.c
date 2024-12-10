
#include <stdio.h>
#include "event/periodic_callback.h"
#include "thread.h"
#include "xtimer.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t queue;
    char stack[STACK_SIZE];

    event_thread_init(&queue, stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1);

    event_periodic_callback_create(&event, ZTIMER_MSEC, 1000, &queue, periodic_callback, NULL);

    event_periodic_callback_start(&event);

    return 0;
}
