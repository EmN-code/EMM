
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    char stack_sender[SENDER_STACKSIZE];
    char stack_receiver[RECEIVER_STACKSIZE];

    sender_pid = thread_create(stack_sender, sizeof(stack_sender),
                               THREAD_PRIORITY_MAIN - 1, 0,
                               sender_thread, NULL, "sender");

    receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver),
                                 THREAD_PRIORITY_MAIN - 2, 0,
                                 receiver_thread, NULL, "receiver");

    return 0;
}
