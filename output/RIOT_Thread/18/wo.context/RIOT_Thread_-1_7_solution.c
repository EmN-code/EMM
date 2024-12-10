#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN + 1)

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t message_queue[8];

void *sender(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        xtimer_sleep(1);
        msg.content.value = 42;
        printf("Sender: Sending message with value %d\n", msg.content.value);
        msg_send(&msg, thread_get(receiver_pid));
    }
    return NULL;
}

void *receiver(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", msg.content.value);
    }
    return NULL;
}

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

int main(void) {
    msg_init_queue(message_queue, 8);
    
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, THREAD_CREATE_STACKTEST, sender, NULL, "Sender Thread");
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, THREAD_CREATE_STACKTEST, receiver, NULL, "Receiver Thread");
    
    printf("Threads started\n");

    return 0;
}