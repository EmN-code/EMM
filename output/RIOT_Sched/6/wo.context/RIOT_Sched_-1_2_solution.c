#include "thread_arch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREAD_STACKSIZE_MAIN (1024 * 4)  // 4KB main thread stack size
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)  // Main thread priority

void *thread_function(void *arg) {
    while (1) {
        printf("Thread running with argument: %s\n", (char *)arg);
        thread_yield();  // Yield control to other threads
    }
    return NULL;
}

int main(void) {
    char thread_stack1[THREAD_STACKSIZE_MAIN];
    char thread_stack2[THREAD_STACKSIZE_MAIN];

    kernel_pid_t thread1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_function, "Thread 1", "thread1");
    if (thread1 <= KERNEL_PID_UNDEF) {
        puts("Error creating thread 1.\n");
        return 1;
    }

    kernel_pid_t thread2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_function, "Thread 2", "thread2");
    if (thread2 <= KERNEL_PID_UNDEF) {
        puts("Error creating thread 2.\n");
        return 1;
    }

    // Change priority of threads during runtime
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}
