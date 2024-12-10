
#include <stdio.h>
#include "thread.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_msg_t evtimer;

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = (evtimer_event_handler_t)periodic_callback;
    event.event.arg = NULL;

    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            evtimer_msg_event_t *received_event = (evtimer_msg_event_t *)msg.content.ptr;
            received_event->event.handler(received_event->event.arg);
        }
    }

    return 0;
}
