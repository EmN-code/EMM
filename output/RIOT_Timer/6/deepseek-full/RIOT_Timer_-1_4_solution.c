
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
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <MQTTClient.h>

#define MSG_QUEUE_SIZE (16)
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;
static mbox_t mbox;

static void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value to indicate timer event
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    evtimer_init(&evtimer);
    evtimer_add_msg(&evtimer, &mbox, (evtimer_msg_event_t){.event = {.offset = TIMER_INTERVAL}, .msg = {.sender_pid = thread_getpid()}});

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.content.value == 1) {
            // Toggle LED logic here
            printf("LED toggled\n");
        }
    }

    return 0;
}
