#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACK_SIZE       (THREAD_STACKSIZE_DEFAULT)

char stack[THREAD_STACK_SIZE];  // Stack for secondary thread

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Message received with content: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_t msg;
    msg.content.value = 42;  // Example message content

    kernel_pid_t thread_pid = thread_create(
        stack, sizeof(stack), THREAD_PRIORITY_MAIN,
        CREATE_STACKTEST, thread_handler, NULL, "thread_handler");

    printf("Thread created with PID: %d\n", thread_pid);

    msg_send(&msg, thread_pid);

    return 0;
}