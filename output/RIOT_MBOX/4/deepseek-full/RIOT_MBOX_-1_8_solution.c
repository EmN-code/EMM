
#include "evtimer_msg.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "timex.h"
#include "ztimer.h"
#include "string.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "xtimer.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void _evtimer_mbox_handler(event_t *event)
{
    evtimer_mbox_event_t *ev = (evtimer_mbox_event_t *)event;
    msg_t msg;
    msg.content.ptr = (void *)ev;
    msg.type = 0;
    mbox_put(ev->mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init(&evtimer, _evtimer_mbox_handler);

    evtimer_mbox_event_t event;
    event.mbox = &mbox;

    xtimer_t timer;
    timer.target = timer.long_target = xtimer_now_timex() + timex_set(1, 0);
    timer.callback = (xtimer_callback_t)evtimer_add;
    timer.arg = &event;

    while (1) {
        xtimer_set(&timer, timex_set(1, 0));
        msg_t msg;
        mbox_get(&mbox, &msg, 1);
        printf("Received message\n");
    }

    return 0;
}
