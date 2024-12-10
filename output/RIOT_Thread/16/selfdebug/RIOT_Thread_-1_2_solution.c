#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "kernel_defines.h"

#define SENDER_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO   (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE       (THREAD_STACKSIZE_MAIN)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];
static msg_t msg_queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;

    while (1) {
        puts("Sender: Sending message.");
        msg_send(&msg, thread_get(RECEIVER_PRIO));
        xtimer_sleep(3);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message of type: 0x%x\n", msg.type);
    }
    return NULL;
}

int main(void) {
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0, receiver, NULL, "receiver");

    return 0;
}