
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "stdio.h"

#define MSG_QUEUE_SIZE (8)
#define SEND_INTERVAL (1000000U)  /* 1 second in microseconds */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        msg.content.value = 1;  /* Example event value */
        msg_send(&msg, thread_getpid());
        xtimer_usleep(SEND_INTERVAL);
    }
}

void receiver_thread(void)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received event with value: %d\n", (int)msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_mbox(&evtimer, &mbox);

    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL, "sender");

    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL, "receiver");

    return 0;
}
