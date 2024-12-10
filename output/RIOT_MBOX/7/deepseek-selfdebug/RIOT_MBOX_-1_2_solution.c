
#include "msg.h"
#include "thread.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 8
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

void *sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        xtimer_sleep(2);
        msg.content.value = 42;
        msg_send(&msg, receiver_pid);
    }

    return NULL;
}

int main(void)
{
    receiver_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        RECEIVER_PRIO,
        THREAD_CREATE_STACKTEST,
        receiver,
        NULL,
        "receiver"
    );

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        SENDER_PRIO,
        THREAD_CREATE_STACKTEST,
        sender,
        NULL,
        "sender"
    );

    return 0;
}
