
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "event/periodic.h"

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

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, periodic_callback, NULL);

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)event_periodic_tick;
    timer.arg = &periodic_event;

    while (1) {
        xtimer_set(&timer, 1000000); // 1 second in microseconds
        msg_receive(NULL);
    }

    return 0;
}
