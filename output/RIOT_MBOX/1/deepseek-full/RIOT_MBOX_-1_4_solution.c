
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_msg(&evtimer, &mbox);

    evtimer_event_t event;
    event.timer = ztimer_now(ZTIMER_MSEC);
    event.callback = event_callback;
    event.arg = NULL;

    ztimer_set(ZTIMER_MSEC, &event.timer, INTERVAL_MS);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, TIME_T_NEVER);
        if (msg.type == EVTIMER_MSG_TYPE) {
            event_callback(msg.content.ptr);
        }
    }

    return 0;
}
