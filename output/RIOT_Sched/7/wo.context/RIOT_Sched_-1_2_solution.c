#include <thread_arch.h>
#include <kernel_defines.h>

void *thread_handler(void *arg) {
    while (1) {
        // Simulate thread work
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t pid1, pid2;

    // Create two threads with initial priorities
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1,
                         CREATE_STACKTEST, thread_handler, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 2,
                         CREATE_STACKTEST, thread_handler, NULL, "thread2");

    // Change priorities of the threads
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN + 3);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 4);

    return 0;
}