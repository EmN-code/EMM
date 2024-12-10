#include "thread_config.h"
#include "sched.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

void *thread_handler_one(void *arg) {
    printf("Thread One is running\n");
    return NULL;
}

void *thread_handler_two(void *arg) {
    printf("Thread Two is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid;
    kernel_pid_t thread_two_pid;

    char thread_one_stack[THREAD_STACKSIZE_MAIN];
    char thread_two_stack[THREAD_STACKSIZE_MAIN];

    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                            THREAD_PRIORITY_MAIN - 1, CREATE_STACKTEST,
                            thread_handler_one, NULL, "thread_one");

    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                            THREAD_PRIORITY_MAIN - 1, CREATE_STACKTEST,
                            thread_handler_two, NULL, "thread_two");

    printf("Changing priorities at runtime\n");

    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN + 1);

    return 0;
}