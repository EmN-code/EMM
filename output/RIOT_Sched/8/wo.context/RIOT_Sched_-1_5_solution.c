#include "thread.h"
#include "kernel.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static char stack_one[STACK_SIZE];
static char stack_two[STACK_SIZE];

void *thread_func_one(void *arg) {
    (void)arg;
    // Implementation of thread function one
    return NULL;
}

void *thread_func_two(void *arg) {
    (void)arg;
    // Implementation of thread function two
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid;
    kernel_pid_t thread_two_pid;

    thread_one_pid = thread_create(stack_one, STACK_SIZE,
                                   THREAD_PRIORITY_MAIN - 1,
                                   THREAD_CREATE_STACKTEST,
                                   thread_func_one, NULL, "thread one");

    thread_two_pid = thread_create(stack_two, STACK_SIZE,
                                   THREAD_PRIORITY_MAIN - 2,
                                   THREAD_CREATE_STACKTEST,
                                   thread_func_two, NULL, "thread two");

    // Change priority of thread one
    sched_change_priority(thread_one_pid, THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread two
    sched_change_priority(thread_two_pid, THREAD_PRIORITY_MAIN - 4);

    return 0;
}