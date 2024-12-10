
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_STACKSIZE_SENDER   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_RECEIVER (THREAD_STACKSIZE_DEFAULT)

#define THREAD_PRIORITY_SENDER    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER  (THREAD_PRIORITY_MAIN - 2)

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

char sender_stack[THREAD_STACKSIZE_SENDER];
char receiver_stack[THREAD_STACKSIZE_RECEIVER];

msg_t msg_queue[8];

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
    msg_init_queue(msg_queue, 8);

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_SENDER,
                               THREAD_CREATE_STACKTEST,
                               sender_thread, NULL, "sender");

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER,
                                 THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");

    return 0;
}
