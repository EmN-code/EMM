#include "ztimer.h"
#include "msg.h"
#include "evtimer_msg.h"
#include <stdio.h>

#define INTERVAL (1U * ZTIMER_MSEC)
#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;

void _event_handler(evtimer_event_t *event) {
    evtimer_msg_event_t *msg_event = (evtimer_msg_event_t *)event;
    printf("Event triggered: %ld\n", ztimer_now(ZTIMER_MSEC));
    msg_send(&msg_event->msg, NULL);
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);

    evtimer_init_msg(&evtimer);

    msg_init_queue(msg_queue, QUEUE_SIZE);

    evtimer_event.event.handler = _event_handler;
    msg_t msg;
    msg.type = 0x1234;  // Example message type
    evtimer_event.msg = msg;

    evtimer_add(&evtimer,
                &evtimer_event.event,
                ztimer_now(ZTIMER_MSEC) + INTERVAL);

    while (1) {
        msg_receive(&msg);
        printf("Message received: %ld\n", ztimer_now(ZTIMER_MSEC));
    }

    return 0;
}