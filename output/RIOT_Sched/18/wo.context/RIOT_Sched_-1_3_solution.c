#include <thread_config.h>
#include <kernel_defines.h>
#include <stdio.h>

void *thread_function_one(void *arg) {
    printf("Thread 1 running\n");
    return NULL;
}

void *thread_function_two(void *arg) {
    printf("Thread 2 running\n");
    return NULL;
}

int main(void) {
    char stack_one[THREAD_STACKSIZE_MAIN];
    char stack_two[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid_one, pid_two;

    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_function_one, NULL, "thread_one");

    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST, thread_function_two, NULL, "thread_two");

    printf("Threads started\n");

    // Change thread priority at runtime
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 1);

    return 0;
}