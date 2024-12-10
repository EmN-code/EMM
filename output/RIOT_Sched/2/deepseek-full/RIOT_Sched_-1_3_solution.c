
#include <stdio.h>
#include <thread.h>
#include <sched.h>

#define STACKSIZE 1024

void *thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
    return NULL;
}

void *thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
    return NULL;
}

int main(void) {
    char stack_1[STACKSIZE];
    char stack_2[STACKSIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN - 1, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN - 2, 0, thread_func_2, NULL, "thread_2");

    printf("Main thread running at priority %d\n", thread_get_priority(thread_get_active()));

    // Change priority of thread 1
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread 1 to %d\n", THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN - 4);
    printf("Changed priority of thread 2 to %d\n", THREAD_PRIORITY_MAIN - 4);

    return 0;
}
