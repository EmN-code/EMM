#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_HIGH 7
#define THREAD_PRIORITY_LOW 10
#define MAIN_QUEUE_SIZE 8

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static msg_t queue[MAIN_QUEUE_SIZE];

void *thread_low_priority(void *arg) {
    msg_t msg;
    (void)arg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message: %ld\n", msg.content.value);
        }
    }

    return NULL;
}

void *thread_high_priority(void *arg) {
    msg_t msg;
    kernel_pid_t target_pid = *((kernel_pid_t *)arg);

    msg.content.value = 42; // Example message content

    msg_send(&msg, target_pid);
    printf("Sent message: %ld\n", msg.content.value);

    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    msg_init_queue(queue, MAIN_QUEUE_SIZE);

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_HIGH, 0, thread_high_priority, &pid2, "thread_high");

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_LOW, 0, thread_low_priority, NULL, "thread_low");

    // Allow threads to run
    while (1) {
        // Main thread work here
    }

    return 0;
}