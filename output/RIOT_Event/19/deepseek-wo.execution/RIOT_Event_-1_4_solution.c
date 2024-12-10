
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdbool.h>
#include <stddef.h>
#include <mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define MSG_QUEUE_SIZE 16

static msg_t msg_queue[MSG_QUEUE_SIZE];

void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_msg_t evtimer;
    evtimer_msg_init(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    kernel_pid_t target_pid = sched_active_pid;

    evtimer_add_msg(&evtimer, &event, target_pid);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            evtimer_msg_event_t *event = (evtimer_msg_event_t *)msg.content.ptr;
            event->event.handler(event->event.arg);
        }
    }

    return 0;
}
