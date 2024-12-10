
#include <stdio.h>
#include "thread.h"
#include "event.h"
#include "ztimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_SEC, &event_thread_queues[EVENT_QUEUE_PRIO_LOWEST], periodic_callback, NULL);

    event_periodic_start(&periodic_event.periodic, 1 * SEC_IN_USEC);

    while (1) {
        event_process(&event_thread_queues[EVENT_QUEUE_PRIO_LOWEST]);
    }

    return 0;
}
