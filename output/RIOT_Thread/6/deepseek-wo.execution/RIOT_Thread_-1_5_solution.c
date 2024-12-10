
#include "kernel_defines.h"
#include "thread.h"
#include "msg.h"
#include "stdio.h"

#define STACKSIZE 2048
#define MSG_QUEUE_SIZE 8

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg)
{
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        msg_send_receive(&msg, &msg, thread_getpid());
        printf("Sender: Sent message with value %u\n", msg.content.value);
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %u\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
