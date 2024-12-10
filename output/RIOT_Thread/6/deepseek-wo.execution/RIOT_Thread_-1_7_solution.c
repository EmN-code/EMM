
#include "msg.h"
#include "thread.h"
#include "stdio.h"

#define STACKSIZE 512
#define NUM_MESSAGES 16

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg)
{
    msg_t msg_array[NUM_MESSAGES];
    msg_init_queue(msg_array, NUM_MESSAGES);

    msg_t msg;
    msg.type = 1;
    msg.content.value = 12345;

    printf("Sender: Sending message with value %lu\n", msg.content.value);
    msg_send(&msg, receiver_pid);

    return NULL;
}

void *receiver_thread(void *arg)
{
    msg_t msg_array[NUM_MESSAGES];
    msg_init_queue(msg_array, NUM_MESSAGES);

    msg_t msg;
    msg_receive(&msg);

    printf("Receiver: Received message with value %lu\n", msg.content.value);

    return NULL;
}

int main(void)
{
    char sender_stack[STACKSIZE];
    char receiver_stack[STACKSIZE];

    receiver_pid = thread_create(receiver_stack, STACKSIZE, 1, 0, receiver_thread, NULL, "receiver");
    sender_pid = thread_create(sender_stack, STACKSIZE, 2, 0, sender_thread, NULL, "sender");

    return 0;
}
