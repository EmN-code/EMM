#include "thread_arch.h"
#include "thread_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *thread_function(void *arg) {
    printf("Thread %d is running with initial priority\n", (int)arg);
    return NULL;
}

void change_thread_priority(kernel_pid_t pid, uint8_t priority) {
    thread_t *thread = thread_get(pid);
    thread->priority = priority;
    printf("Priority of thread %d changed to %d\n", pid, priority);
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    // Start the first thread
    thread1_pid = thread_create((char *)malloc(THREAD_STACKSIZE_DEFAULT), THREAD_STACKSIZE_DEFAULT,
                                THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_WOUT_YIELD,
                                thread_function, (void *)1, "thread1");
    if (thread1_pid < 0) {
        printf("Failed to create thread 1\n");
        return -1;
    }

    // Start the second thread
    thread2_pid = thread_create((char *)malloc(THREAD_STACKSIZE_DEFAULT), THREAD_STACKSIZE_DEFAULT,
                                THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_WOUT_YIELD,
                                thread_function, (void *)2, "thread2");
    if (thread2_pid < 0) {
        printf("Failed to create thread 2\n");
        return -1;
    }

    printf("Threads started\n");

    // Change priority of first thread
    change_thread_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);

    // Change priority of second thread
    change_thread_priority(thread2_pid, THREAD_PRIORITY_MAIN - 3);

    // Allow threads to run
    thread_wakeup(thread1_pid);
    thread_wakeup(thread2_pid);

    while (1) {
        // Main loop doing other tasks
    }

    return 0;
}