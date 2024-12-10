#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE 1024
#define MESSAGE_QUEUE_SIZE 8

char sender_stack[STACKSIZE];
char receiver_stack[STACKSIZE];
msg_t message_queue[MESSAGE_QUEUE_SIZE];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void) arg;
    msg_t msg;
    msg.content.value = 42;
    msg_send(&msg, receiver_pid);
    return NULL;
}

void *receiver(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg)) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);
    
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_SENDER, 0,
                               sender, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread\n");
        return 1;
    }

    return 0;
}