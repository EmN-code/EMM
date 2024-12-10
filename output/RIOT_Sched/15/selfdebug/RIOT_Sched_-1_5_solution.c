#include "thread_config.h"
#include "kernel_defines.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_ONE (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_TWO (THREAD_PRIORITY_MAIN - 2)

char stack_one[STACK_SIZE];
char stack_two[STACK_SIZE];

kernel_pid_t thread_one;
kernel_pid_t thread_two;

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread work
    }
    return NULL;
}

int main(void) {
    thread_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_ONE, 0, thread_func, NULL, "thread_one");
    if (thread_one <= KERNEL_PID_UNDEF) {
        // Handle error
    }

    thread_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_TWO, 0, thread_func, NULL, "thread_two");
    if (thread_two <= KERNEL_PID_UNDEF) {
        // Handle error
    }

    // Change priority of threads after creation
    sched_change_priority(thread_get(thread_one), THREAD_PRIORITY_MAIN);
    sched_change_priority(thread_get(thread_two), THREAD_PRIORITY_MAIN + 1);

    return 0;
}