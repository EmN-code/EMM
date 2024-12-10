
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
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    ztimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;
    timer.target = ztimer_now(ZTIMER_MSEC) + ZTIMER_MSEC_TO_TICKS(1000);
    timer.long_target = 0;

    while (1) {
        ztimer_set(ZTIMER_MSEC, &timer, 1000);
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
