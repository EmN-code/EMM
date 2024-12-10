
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph_conf.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

char stack_sender[STACKSIZE];
char stack_receiver[STACKSIZE];

kernel_pid_t pid_sender;
kernel_pid_t pid_receiver;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, pid_receiver);
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
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    pid_sender = thread_create(stack_sender, sizeof(stack_sender), PRIORITY_SENDER,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    pid_receiver = thread_create(stack_receiver, sizeof(stack_receiver), PRIORITY_RECEIVER,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
