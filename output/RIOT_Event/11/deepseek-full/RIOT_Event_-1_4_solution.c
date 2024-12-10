
#include "msg.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "timex.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define MSG_QUEUE_SIZE 16

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void periodic_callback(void *arg)
{
    printf("Periodic callback triggered\n");
    msg_t msg;
    msg.content.value = 0;
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        periodic_callback(NULL);
    }

    return 0;
}
