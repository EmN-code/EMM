
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_STACKSIZE 512
#define MSG_QUEUE_SIZE 10

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1); // Sleep for 1 second between messages
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    char stack_sender[THREAD_STACKSIZE];
    char stack_receiver[THREAD_STACKSIZE];

    sender_pid = thread_create(stack_sender, sizeof(stack_sender),
                               THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");

    receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver),
                                 THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    return 0;
}