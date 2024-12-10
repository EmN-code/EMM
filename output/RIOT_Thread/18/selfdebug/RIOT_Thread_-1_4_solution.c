#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY_1 (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_2 (THREAD_PRIORITY_MAIN - 2)

static char stack1[STACK_SIZE];
static char stack2[STACK_SIZE];

void *thread_function_1(void *arg) {
    msg_t msg;
    msg.content.value = 0xDEADBEEF;
    kernel_pid_t receiver_pid = (kernel_pid_t)arg;

    printf("Thread 1: Sending message...\n");
    msg_send(&msg, receiver_pid);
    printf("Thread 1: Message sent!\n");

    return NULL;
}

void *thread_function_2(void *arg) {
    msg_t msg;

    printf("Thread 2: Waiting for message...\n");
    msg_receive(&msg);
    printf("Thread 2: Received message with value: 0x%08" PRIx32 "\n", msg.content.value);

    return NULL;
}

int main(void) {
    kernel_pid_t thread_1_pid = thread_create(stack1, STACK_SIZE, PRIORITY_1, 0, thread_function_1, (void *)thread_getpid(), "thread_1");
    if (thread_1_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 1\n");
        return 1;
    }

    kernel_pid_t thread_2_pid = thread_create(stack2, STACK_SIZE, PRIORITY_2, 0, thread_function_2, NULL, "thread_2");
    if (thread_2_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 2\n");
        return 1;
    }

    return 0;
}