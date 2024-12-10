#include <stdio.h>
#include <sched.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

static void *thread_func_one(void *arg) {
    (void)arg;
    printf("Thread one executing\n");
    return NULL;
}

static void *thread_func_two(void *arg) {
    (void)arg;
    printf("Thread two executing\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one;
    kernel_pid_t pid_two;

    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN, 0, thread_func_one, NULL, "thread_one");
    if (pid_one <= KERNEL_PID_UNDEF) {
        puts("Error creating thread one");
    }

    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN, 0, thread_func_two, NULL, "thread_two");
    if (pid_two <= KERNEL_PID_UNDEF) {
        puts("Error creating thread two");
    }

    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN + 2);

    return 0;
}