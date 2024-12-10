#include <stdio.h>
#include "msg.h"
#include "kernel.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define QUEUE_SIZE               (8)

static kernel_pid_t receiver_pid;
static msg_t msg_queue[QUEUE_SIZE];

void *receiver(void *arg) {
    msg_t msg;
    (void)arg;
    printf("Receiver thread started.\n");

    while (1) {
        msg_receive(&msg);
        if (msg.type == 1) {
            printf("Received message: %ld\n", msg.content.value);
        }
    }

    return NULL;
}

void *sender(void *arg) {
    msg_t msg;
    (void)arg;
    printf("Sender thread started.\n");

    for (int i = 0; i < 10; i++) {
        msg.type = 1;
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        printf("Sent message: %d\n", i);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER,
                                 0, receiver, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER,
                  0, sender, NULL, "sender");

    printf("Main thread exiting\n");
    return 0;
}