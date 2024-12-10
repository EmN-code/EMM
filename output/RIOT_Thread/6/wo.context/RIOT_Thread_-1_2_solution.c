#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define SENDER_PRIO THREAD_PRIORITY_MAIN - 1
#define RECEIVER_PRIO THREAD_PRIORITY_MAIN - 2
#define STACKSIZE 512

char sender_stack[STACKSIZE];
char receiver_stack[STACKSIZE];

void *sender_thread(void *arg) {
    msg_t msg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg.type = 1;
    msg.content.value = 42;

    printf("Sender: Sending message to PID %u\n", (unsigned int)receiver_pid);
    msg_send(&msg, receiver_pid);
    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg;

    printf("Receiver: Waiting for message...\n");
    msg_receive(&msg);
    printf("Receiver: Received message with type %u and value %lu\n", (unsigned int)msg.type, (unsigned long)msg.content.value);
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, STACKSIZE, RECEIVER_PRIO, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, STACKSIZE, SENDER_PRIO, THREAD_CREATE_STACKTEST, sender_thread, (void *)&receiver_pid, "sender");
    
    return 0;
}