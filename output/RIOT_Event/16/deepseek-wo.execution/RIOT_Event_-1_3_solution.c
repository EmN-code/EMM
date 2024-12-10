
#include "msg.h"
#include "ztimer.h"
#include "timex.h"
#include "mbox.h"
#include "evtimer_mbox.h"

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init(&evtimer);

    ztimer_t periodic_timer;
    periodic_timer.callback = periodic_callback;
    periodic_timer.arg = NULL;

    ztimer_set(ZTIMER_MSEC, &periodic_timer, 1000);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
