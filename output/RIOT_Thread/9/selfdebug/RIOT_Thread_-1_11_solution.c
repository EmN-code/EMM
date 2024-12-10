#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char sender_stack[SENDER_THREAD_STACKSIZE];
char receiver_stack[RECEIVER_THREAD_STACKSIZE];

kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    printf("Sender: Sending message with value %d.\n", msg.content.value);
    msg_send(&msg, receiver_pid);
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Receiver: Received message with value %d.\n", msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_t *array, 8);

    receiver_pid = thread_create(receiver_stack, RECEIVER_THREAD_STACKSIZE,
                                 RECEIVER_THREAD_PRIORITY, 0,
                                 receiver, NULL, "receiver");

    thread_create(sender_stack, SENDER_THREAD_STACKSIZE,
                  SENDER_THREAD_PRIORITY, 0,
                  sender, NULL, "sender");

    /* Let the threads run */
    while(1) {}

    return 0;
}