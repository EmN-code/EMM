#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_SENDER THREAD_STACKSIZE_MAIN
#define THREAD_STACKSIZE_RECEIVER THREAD_STACKSIZE_MAIN

static char sender_stack[THREAD_STACKSIZE_SENDER];
static char receiver_stack[THREAD_STACKSIZE_RECEIVER];

static msg_t msg_queue[8];
static msg_t msg;
static kernel_pid_t receiver_pid;

void* sender_thread(void *arg) {
    (void) arg;
    msg.content.value = 42;
    printf("Sender: Sending message with value %d\n", (int)msg.content.value);
    msg_send(&msg, receiver_pid);
    return NULL;
}

void* receiver_thread(void *arg) {
    (void) arg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", (int)msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, THREAD_STACKSIZE_RECEIVER, THREAD_PRIORITY_RECEIVER, 0,
                                 receiver_thread, NULL, "receiver");
    thread_create(sender_stack, THREAD_STACKSIZE_SENDER, THREAD_PRIORITY_SENDER, 0,
                  sender_thread, NULL, "sender");
    return 0;
}