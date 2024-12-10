#include "thread.h"
#include "sched.h"
#include "stdio.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void* thread_func(void *arg) {
    printf("Thread %ld is running\n", (long)arg);
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stack1, STACK_SIZE, PRIORITY, 0, thread_func, (void*)1, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, STACK_SIZE, PRIORITY, 0, thread_func, (void*)2, "thread2");

    printf("Created thread with PID %d\n", pid1);
    printf("Created thread with PID %d\n", pid2);

    // change priority of pid1
    sched_change_priority(thread_get(pid1), PRIORITY + 1);
    // change priority of pid2
    sched_change_priority(thread_get(pid2), PRIORITY + 2);

    return 0;
}