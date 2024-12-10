
#include "msg.h"
#include "thread.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 16
#define SENDER_THREAD_STACKSIZE 512
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL_MS 1000000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t sender_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(INTERVAL_MS / 1000000);
        msg_send(&msg, sender_pid);
    }

    return NULL;
}

int main(void)
{
    mbox_t mbox = MBOX_INIT(msg_queue, MSG_QUEUE_SIZE);
    msg_t msg;

    sender_pid = thread_create(
        (char *)"sender_thread",
        SENDER_THREAD_STACKSIZE,
        SENDER_THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender_thread"
    );

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
