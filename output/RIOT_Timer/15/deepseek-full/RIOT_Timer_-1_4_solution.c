
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
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"

#define LED_PIN 5
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Signal to toggle LED
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.content.value == 1) {
            // Toggle LED logic here
            printf("LED toggled\n");
        }
        ztimer_set(ZTIMER_MSEC, &timer, 1000);
    }

    return 0;
}
