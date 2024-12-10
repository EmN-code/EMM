#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>

#define STACK_SIZE 1024

char stack_thread1[STACK_SIZE];
char stack_thread2[STACK_SIZE];

void *thread_func1(void *arg) {
    printf("Thread 1 running\n");
    while (1) {
        // Simulated work
    }
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 running\n");
    while (1) {
        // Simulated work
    }
    return NULL;
}

int main() {
    kernel_pid_t pid1, pid2;

    // Create two threads
    pid1 = thread_create(stack_thread1, STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_WOUT_YIELD, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack_thread2, STACK_SIZE, THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_WOUT_YIELD, thread_func2, NULL, "thread2");

    // Change priority at runtime
    thread_set_priority(pid1, THREAD_PRIORITY_MAIN - 3);
    thread_set_priority(pid2, THREAD_PRIORITY_MAIN);

    return 0;
}
