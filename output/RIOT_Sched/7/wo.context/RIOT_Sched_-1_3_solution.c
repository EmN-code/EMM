#include <thread_arch.h>
#include <kernel_defines.h>
#include <irq_arch.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define PRIORITY_THREAD1 (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD2 (THREAD_PRIORITY_MAIN - 2)

void *thread_func1(void *arg) {
    // Add code for thread 1
    return NULL;
}

void *thread_func2(void *arg) {
    // Add code for thread 2
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, STACKSIZE, PRIORITY_THREAD1, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, PRIORITY_THREAD2, 0, thread_func2, NULL, "thread2");

    // Change priority of thread 1
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN + 1);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 1);

    return 0;
}