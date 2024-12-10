#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include "architecture.h"

void *thread1(void *arg) {
    // thread1 implementation
    return NULL;
}

void *thread2(void *arg) {
    // thread2 implementation
    return NULL;
}

int main(void) {
    // Buffers for thread stacks
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    // Create two threads
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread2, NULL, "thread2");

    // Change the priority of the first thread
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);

    // Change the priority of the second thread
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}