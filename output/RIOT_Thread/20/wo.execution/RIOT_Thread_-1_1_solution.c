#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10
#define THREAD_STACKSIZE_MAIN 1024

char stack_high[THREAD_STACKSIZE_MAIN];
char stack_low[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread_high_pid;
kernel_pid_t thread_low_pid;

void *thread_high(void *arg) {
    msg_t msg;
    (void)arg;

    msg.content.value = 42; // set message content
    msg.type = 1; // message type

    // Send the message to the low priority thread
    if (msg_send(&msg, thread_low_pid) < 0) {
        printf("Message could not be sent to low priority thread\n");
    } else {
        printf("Message sent successfully to low priority thread\n");
    }

    return NULL;
}

void *thread_low(void *arg) {
    msg_t msg;
    (void)arg;

    // Initialize message queue for this thread
    msg_init_queue(msg.data, sizeof(msg.data));

    while (1) {
        // Block and wait to receive messages
        msg_receive(&msg);
        printf("Low priority thread received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    thread_low_pid = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW, 0, thread_low, NULL, "low");
    if (thread_low_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating low priority thread\n");
        return -1;
    }

    thread_high_pid = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH, 0, thread_high, NULL, "high");
    if (thread_high_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating high priority thread\n");
        return -1;
    }

    // Let the threads run indefinitely
    while (1) {}

    return 0;
}