
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdatomic.h"

#define MSG_QUEUE_SIZE (16)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
    msg_t msg;
    msg.content.value = 0; // Dummy value
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    ztimer_t periodic_timer;
    periodic_timer.callback = periodic_callback;
    periodic_timer.arg = NULL;
    periodic_timer.interval = ZTIMER_USEC(1000000); // 1 second

    ztimer_set(ZTIMER_USEC, &periodic_timer, periodic_timer.interval);

    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Message received!\n");
        }
    }

    return 0;
}
