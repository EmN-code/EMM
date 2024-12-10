#include "thread.h"
#include "sched.h"

void *thread1_handler(void *arg) {
    // Infinite loop for thread1
    while (1) {
        // Simulate thread work
        thread_yield();
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    // Infinite loop for thread2
    while (1) {
        // Simulate thread work
        thread_yield();
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread1_pid, thread2_pid;

    // Create thread 1
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_handler, NULL, "thread1");
                                
    // Create thread 2
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_handler, NULL, "thread2");

    // Change thread2 priority at runtime
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);

    return 0;
}