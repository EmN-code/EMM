#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack_one[THREAD_STACKSIZE];
char stack_two[THREAD_STACKSIZE];

void *thread_one_func(void *arg) {
    puts("Thread one is running.");
    return NULL;
}

void *thread_two_func(void *arg) {
    puts("Thread two is running.");
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(
        stack_one, sizeof(stack_one),
        THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
        thread_one_func, NULL, "thread_one");

    kernel_pid_t thread_two_pid = thread_create(
        stack_two, sizeof(stack_two),
        THREAD_PRIORITY + 1, THREAD_CREATE_STACKTEST,
        thread_two_func, NULL, "thread_two");

    // Change the priority of thread two
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY - 2);

    // Ensuring thread_one_pid is used to avoid warnings
    printf("Thread one PID: %d\n", thread_one_pid);

    while (1) {
        // Main thread idle
    }

    return 0;
}