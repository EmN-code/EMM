
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <msg.h>

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void event_periodic_callback(void *arg)
{
    printf("Event periodic callback triggered!\n");
    (void)arg;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_init(&evtimer, event_periodic_callback);

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)evtimer_add_msg;
    timer.arg = &evtimer;

    while (1) {
        xtimer_set(&timer, 1 * US_PER_SEC);
        msg_t msg;
        msg_receive(&msg);
        printf("Message received!\n");
    }

    return 0;
}
