
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 16
#define SENDER_THREAD_STACKSIZE 512
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static kernel_pid_t mbox_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, mbox_pid);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    mbox_pid = thread_create(
        NULL,
        SENDER_THREAD_STACKSIZE,
        SENDER_THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender_thread"
    );

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
