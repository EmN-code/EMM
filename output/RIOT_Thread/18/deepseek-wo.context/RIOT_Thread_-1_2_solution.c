
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_STACKSIZE_SENDER (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_RECEIVER (THREAD_STACKSIZE_DEFAULT)

#define PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 12345;

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
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    char stack_buffer_sender[THREAD_STACKSIZE_SENDER];
    char stack_buffer_receiver[THREAD_STACKSIZE_RECEIVER];

    sender_pid = thread_create(stack_buffer_sender, sizeof(stack_buffer_sender), PRIORITY_SENDER,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    receiver_pid = thread_create(stack_buffer_receiver, sizeof(stack_buffer_receiver), PRIORITY_RECEIVER,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
