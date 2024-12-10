#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "msg.h"
#include "thread.h"

#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE        THREAD_STACKSIZE_DEFAULT

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static msg_t msg_queue[4];
static kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    printf("[Sender] Sending: %d\n", msg.content.value);
    msg_send(&msg, receiver_pid);
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg, reply;
    msg.content.value = 0; // Initialize with zero
    msg_receive(&msg);
    printf("[Receiver] Received: %d\n", msg.content.value);
    reply.content.value = msg.content.value + 1;
    msg_reply(&msg, &reply);
    printf("[Receiver] Sent reply: %d\n", reply.content.value);
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver, NULL, "receiver");
    printf("Receiver thread PID: %" PRIkernel_pid_t "\n", receiver_pid);

    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0,
                  sender, NULL, "sender");
    return 0;
}