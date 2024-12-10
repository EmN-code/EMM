
#include "thread.h"
#include "msg.h"
#include "evtimer.h"
#include "ztimer.h"
#include "mbox.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static msg_t mbox_queue[8];

static evtimer_t evtimer;

void event_callback(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_queue, sizeof(mbox_queue) / sizeof(mbox_queue[0]));

    evtimer_init(&evtimer);

    evtimer_event_t event;
    evtimer_add_msg(&evtimer, &event, &mbox, (msg_t *)event_callback, NULL, ZTIMER_MSEC, 1000);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        ((void (*)(void *))msg.content.ptr)(NULL);
    }

    return 0;
}
