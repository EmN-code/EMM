#include <thread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_PRIORITY_MAIN     (6)
#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)
#define THREAD_STACKSIZE_LARGE   (THREAD_STACKSIZE_DEFAULT * 2)

char sender_stack[THREAD_STACKSIZE_LARGE];
char receiver_stack[THREAD_STACKSIZE_LARGE];

msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        printf("Sender: Sending message with content 42\n");
        msg.content.value = 42;
        msg_send(&msg, thread_getpid_of(receiver_stack));
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with content: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    if (sender_pid < 0) {
        printf("Failed to create sender thread\n");
        return 1;
    }

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN, 0, receiver_thread, NULL, "receiver");
    if (receiver_pid < 0) {
        printf("Failed to create receiver thread\n");
        return 1;
    }

    return 0;
}