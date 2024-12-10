#include <thread_config.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <kernel_types.h>

#define STACKSIZE THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY2 (THREAD_PRIORITY_MAIN - 2)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_func1(void *arg) {
    (void)arg; // unused
    printf("Thread 1 running\n");
    while (1) {
        // Thread 1 does its work here
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg; // unused
    printf("Thread 2 running\n");
    while (1) {
        // Thread 2 does its work here
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY1,
                                      THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    printf("Thread 1 created with PID %d\n", pid1);

    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY2,
                                      THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");
    printf("Thread 2 created with PID %d\n", pid2);

    // Change the priority of threads during runtime
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread 1\n");

    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 4);
    printf("Changed priority of thread 2\n");

    return 0;
}