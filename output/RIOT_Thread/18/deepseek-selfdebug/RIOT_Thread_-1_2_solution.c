
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MAIN_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MAIN_THREAD_PRIORITY (THREAD_PRIORITY_MAIN)

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

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
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    char sender_stack[SENDER_THREAD_STACKSIZE];
    char receiver_stack[RECEIVER_THREAD_STACKSIZE];

    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_THREAD_PRIORITY,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
