#include <thread_arch.h>
#include <kernel_defines.h>

#include <stdio.h>
#include <stdint.h>

#define THREAD_STACKSIZE  (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)

void *thread1_handler(void *arg) {
    printf("Thread 1 is running\n");
    while (1) {
        // Thread 1 work here
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    printf("Thread 2 is running\n");
    while (1) {
        // Thread 2 work here
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE];
    char stack2[THREAD_STACKSIZE];
    kernel_pid_t thread_pid1, thread_pid2;

    thread_pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread1_handler, NULL, "thread1");
    thread_pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY, THREAD_CREATE_STACKTEST, thread2_handler, NULL, "thread2");

    printf("Threads created\n");

    sched_change_priority(thread_get(thread_pid1), THREAD_PRIORITY + 1);
    sched_change_priority(thread_get(thread_pid2), THREAD_PRIORITY - 1);

    return 0;
}