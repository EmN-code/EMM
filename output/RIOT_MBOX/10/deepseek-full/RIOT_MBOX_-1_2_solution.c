
#include <stdio.h>
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define QUEUE_SIZE 8
#define SENDER_THREAD_STACKSIZE 512
#define SENDER_THREAD_PRIO 1

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static kernel_pid_t sender_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        msg_send(&msg, sender_pid);
        xtimer_sleep(1); // Send a message every second
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    sender_pid = thread_create(
        (char *)"sender_thread",
        SENDER_THREAD_STACKSIZE,
        SENDER_THREAD_PRIO,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender_thread"
    );

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
