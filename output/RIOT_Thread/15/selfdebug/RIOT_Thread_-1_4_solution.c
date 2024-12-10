#include <thread.h>
#include <stdio.h>
#include "msg.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN)

#define QUEUE_SIZE (8)

static char sender_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_stack[THREAD_STACKSIZE_DEFAULT];
msg_t msg_queue[QUEUE_SIZE];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    printf("Sender: Sending message\n");
    msg_send(&msg, receiver_pid);
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    printf("Receiver: Waiting for message\n");
    msg_receive(&msg);
    printf("Receiver: Received message with content: %ld\n", msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER,
                                 THREAD_CREATE_STACKTEST,
                                 receiver, NULL, "receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_SENDER,
                               THREAD_CREATE_STACKTEST,
                               sender, NULL, "sender");

    return 0;
}