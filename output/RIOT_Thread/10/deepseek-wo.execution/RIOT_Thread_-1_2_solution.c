
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

char sender_stack[STACKSIZE];
char receiver_stack[STACKSIZE];

msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, receiver_pid);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    sender_pid = thread_create(sender_stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    receiver_pid = thread_create(receiver_stack, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    return 0;
}
