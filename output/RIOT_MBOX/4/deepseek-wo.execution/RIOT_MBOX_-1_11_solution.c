
#include "evtimer_msg.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "mma8x5x_regs.h"

#define QUEUE_SIZE (16)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void _evtimer_mbox_handler(void *arg)
{
    evtimer_event_t *event = (evtimer_event_t *)arg;
    msg_t msg;
    msg.content.ptr = event;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    evtimer_event_t event;
    event.event.handler = _evtimer_mbox_handler;
    event.event.arg = &event;

    ztimer_set(ZTIMER_MSEC, &event.timer, 1000);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, 1);
        evtimer_event_t *received_event = (evtimer_event_t *)msg.content.ptr;
        printf("Event received!\n");
        ztimer_set(ZTIMER_MSEC, &received_event->timer, 1000);
    }

    return 0;
}
