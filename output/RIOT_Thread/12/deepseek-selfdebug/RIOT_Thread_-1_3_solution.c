
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    (void)arg;
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
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    char sender_stack[SENDER_STACKSIZE];
    char receiver_stack[RECEIVER_STACKSIZE];

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_MAIN - 1, 0,
                               sender_thread, NULL, "sender");

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN - 2, 0,
                                 receiver_thread, NULL, "receiver");

    return 0;
}
