#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACK_SIZE       (THREAD_STACKSIZE_MAIN)
#define PRIORITY_SENDER  (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

char sender_stack[STACK_SIZE];
char receiver_stack[STACK_SIZE];

kernel_pid_t receiver_pid;

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(&msg, 1);

    while (1) {
        msg_receive(&msg);
        printf("Received message of type: %u\n", msg.type);
    }

    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    printf("Starting inter-thread communication example\n");

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), PRIORITY_RECEIVER, 0, receiver, NULL, "receiver");

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), PRIORITY_SENDER, 0, sender, NULL, "sender");

    (void)sender_pid;  // Explicitly mark sender_pid as unused

    return 0;
}