#include "thread.h"
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)

static char sender_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_stack[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234; // Example message type
    while (1) {
        msg_send(&msg, thread_get(SENDER_THREAD_PID));
        printf("Message sent from sender thread\n");
        thread_yield();
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received in receiver thread with type: %04x\n", msg.type);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t sender_thread_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, CREATE_STACKTEST, sender_thread, NULL, "sender");
    if (sender_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread\n");
        return 1;
    }

    kernel_pid_t receiver_thread_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 2, CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    if (receiver_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    return 0;
}