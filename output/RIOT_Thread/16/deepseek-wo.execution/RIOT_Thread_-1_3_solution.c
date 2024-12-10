
#include "msg.h"
#include "thread.h"
#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"

#define STACKSIZE 1024
#define MSG_QUEUE_SIZE 8

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;
    msg.type = 1;
    msg.content.value = 12345;

    for (int i = 0; i < 5; i++) {
        msg_send(&msg, receiver_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;

    for (int i = 0; i < 5; i++) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    receiver_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    sender_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    return 0;
}
