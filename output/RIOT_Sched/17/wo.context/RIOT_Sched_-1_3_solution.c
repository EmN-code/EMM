#include <thread_config.h>
#include <kernel_defines.h>
#include <stdlib.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

void *thread_func_one(void *arg) {
    while(1) {
        printf("Thread 1 executing\n");
        thread_yield();
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    while(1) {
        printf("Thread 2 executing\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    char stack1[STACK_SIZE];
    char stack2[STACK_SIZE];

    kernel_pid_t pid1 = thread_create(stack1, STACK_SIZE, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_func_one, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func_two, NULL, "thread2");

    // Changing priority at runtime
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 2);

    while (1) {
        // main thread yields to allow other threads to execute
        thread_yield();
    }

    return 0;
}