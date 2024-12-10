
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

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
    char stack_sender[THREAD_STACKSIZE_DEFAULT];
    char stack_receiver[THREAD_STACKSIZE_DEFAULT];

    receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver),
                                 THREAD_PRIORITY_MAIN + 1, 0,
                                 receiver_thread, NULL, "receiver");

    sender_pid = thread_create(stack_sender, sizeof(stack_sender),
                               THREAD_PRIORITY_MAIN + 2, 0,
                               sender_thread, NULL, "sender");

    return 0;
}
