#include "ztimer.h"
#include "thread.h"
#include "thread_config.h"
#include "sched.h"
#include "stdio.h"
#include "stdlib.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack_one[THREAD_STACKSIZE];
char stack_two[THREAD_STACKSIZE];

void *thread_func_one(void *arg) {
    puts("This is thread one.");
    return NULL;
}

void *thread_func_two(void *arg) {
    puts("This is thread two.");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY, 0,
                           thread_func_one, NULL, "thread_one");

    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY, 0,
                           thread_func_two, NULL, "thread_two");

    // Change priority at runtime
    thread_t *thread = thread_get(pid_one);
    sched_change_priority(thread, THREAD_PRIORITY_MAIN - 2);

    thread = thread_get(pid_two);
    sched_change_priority(thread, THREAD_PRIORITY_MAIN - 3);

    return 0;
}