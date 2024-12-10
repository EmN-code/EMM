
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sched.h>
#include <thread.h>

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

void *thread_func1(void *arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_func2, NULL, "thread2");

    // Change priority of thread 1
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN + 1);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 2);

    printf("Main thread is running\n");

    return 0;
}
