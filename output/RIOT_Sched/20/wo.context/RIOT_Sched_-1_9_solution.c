#include <thread_config.h>
#include <stdio.h>
#include <stdint.h>
#include <kernel_defines.h>

#define STACKSIZE_DEFAULT (2048)

typedef void *(*thread_task_func_t)(void *arg);

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    printf("Thread One is running\n");
    while (1) {}
    return NULL;
}

void *thread_two(void *arg) {
    printf("Thread Two is running\n");
    while (1) {}
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE_DEFAULT];
    char stack2[STACKSIZE_DEFAULT];

    thread_one_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING, thread_one, NULL, "thread_one");
    thread_two_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING, thread_two, NULL, "thread_two");

    sched_change_priority(thread_one_pid, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_two_pid, THREAD_PRIORITY_MAIN - 1);

    thread_wakeup(thread_one_pid);
    thread_wakeup(thread_two_pid);

    return 0;
}
