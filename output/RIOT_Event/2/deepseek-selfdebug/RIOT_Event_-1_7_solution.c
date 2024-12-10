
#include <stdio.h>
#include "thread.h"
#include "event/callback.h"
#include "event/periodic.h"
#include "event/timeout.h"
#include "ztimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY);

    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback, NULL);
    event_periodic_callback_start(&periodic_event, 1000000); // 1 second interval

    return 0;
}
