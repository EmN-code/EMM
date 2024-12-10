
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <MQTTClient.h>

#define MSG_QUEUE_SIZE (16)
#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.msg.sender_pid = thread_getpid();
    event.msg.type = 0;
    event.msg.content.ptr = NULL;

    ztimer_t timer;
    timer.callback = (void (*)(void *))periodic_callback;
    timer.arg = NULL;
    timer.target_pid = thread_getpid();

    while (1) {
        ztimer_set(ZTIMER_SEC, &timer, PERIODIC_INTERVAL);
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == 0) {
            periodic_callback(msg.content.ptr);
        }
    }

    return 0;
}
