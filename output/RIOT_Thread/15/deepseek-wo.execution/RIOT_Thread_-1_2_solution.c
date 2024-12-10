
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <xtimer.h>

#define STACKSIZE 1024
#define MSG_QUEUE_SIZE 8

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;
    msg.content.value = 123;
    msg.type = 1;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    sender_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    receiver_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
