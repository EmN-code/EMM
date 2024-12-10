
#include <stdlib.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_msg_t evtimer;
    evtimer_msg_init(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = (evtimer_callback_t)periodic_callback;
    event.event.arg = NULL;

    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second
    evtimer_add_msg(&evtimer, &event, sched_active_pid);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            evtimer_msg_event_t *received_event = (evtimer_msg_event_t *)msg.content.ptr;
            received_event->event.handler(received_event->event.arg);
            evtimer_add_msg(&evtimer, received_event, sched_active_pid);
        }
    }

    return 0;
}
