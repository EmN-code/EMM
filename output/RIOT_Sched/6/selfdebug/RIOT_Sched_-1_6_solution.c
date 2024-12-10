#include <thread.h>
#include <sched.h>
#include <stdio.h>

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_func_one(void *arg) {
    printf("Thread One is running\n");
    return NULL;
}

void *thread_func_two(void *arg) {
    printf("Thread Two is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                                         THREAD_CREATE_STACKTEST, thread_func_one, NULL, "thread_one");
    kernel_pid_t pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 1,
                                         THREAD_CREATE_STACKTEST, thread_func_two, NULL, "thread_two");

    printf("Threads have been created\n");

    // Change priority of the threads
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 2);

    printf("Thread priorities have been changed\n");

    // Keep the main thread alive
    thread_sleep();

    return 0;
}