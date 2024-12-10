#include "thread.h"
#include "kernel_defines.h"
#include "inttypes.h"

void *thread_function(void *arg) {
    while (1) {
        // Simulated workload
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread1 = thread_create(stack1, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_function, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_function, NULL, "thread2");

    // Threads have been created with specific priorities
    // Changing priorities at runtime
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}