
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "evtimer.h"

#define MSG_QUEUE_SIZE     (8)
#define EVENT_INTERVAL     (1000000U)  /* 1 second */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;

void event_callback(void *arg)
{
    msg_t msg;
    msg.content.ptr = "Event triggered";
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init(&evtimer);

    evtimer_add_msg(&evtimer, EVENT_INTERVAL, event_callback, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        puts((char *)msg.content.ptr);
    }

    return 0;
}
