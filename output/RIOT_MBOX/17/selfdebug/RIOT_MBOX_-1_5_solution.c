#include "xtimer.h"
#include "evtimer.h"
#include "msg.h"
#include "mbox.h"
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)

static void _evtimer_mbox_handler(evtimer_event_t *event) {
    printf("Event Timer Triggered!\n");
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    mbox_t mbox;
    msg_t mbox_queue[8];
    mbox_init(&mbox, mbox_queue, 8);

    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_init(&evtimer, _evtimer_mbox_handler);

    event.period = TIMER_INTERVAL;
    event.mbox = &mbox;
    event.msg.type = 0;
    evtimer_add(&evtimer, &event);

    printf("Event timer configured with mailbox!\n");

    while (1) {
        msg_t msg;
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Message received from mailbox: type = %d\n", msg.type);
        }
    }

    return 0;
}