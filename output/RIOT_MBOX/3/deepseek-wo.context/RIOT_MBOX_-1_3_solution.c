
#include <stdio.h>
#include "thread.h"
#include "evtimer.h"
#include "msg.h"
#include "ztimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init(&evtimer);

    event.event.handler = event_handler;
    event.event.arg = NULL;
    event.msg.type = 0;
    event.msg.content.ptr = NULL;

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == 0) {
            printf("Message received!\n");
        }
    }

    return 0;
}
