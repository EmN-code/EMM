#include <thread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACKSIZE  THREAD_STACKSIZE_DEFAULT

char thread_1_stack[THREAD_STACKSIZE];
char thread_2_stack[THREAD_STACKSIZE];

void *thread_1_handler(void *arg) {
    while (1) {
        printf("Thread 1 Running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_2_handler(void *arg) {
    while (1) {
        printf("Thread 2 Running\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    // Create two threads
    kernel_pid_t thread_1_pid = thread_create(thread_1_stack, sizeof(thread_1_stack), THREAD_PRIORITY_MAIN - 1, CREATE_STACKTEST, thread_1_handler, NULL, "thread_1");
    kernel_pid_t thread_2_pid = thread_create(thread_2_stack, sizeof(thread_2_stack), THREAD_PRIORITY_MAIN - 2, CREATE_STACKTEST, thread_2_handler, NULL, "thread_2");

    // Change priority of threads
    thread_t *thread_1 = thread_get(thread_1_pid);
    thread_t *thread_2 = thread_get(thread_2_pid);

    // Error Handling
    if (!thread_1 || !thread_2) {
        printf("Error: Thread creation failed\n");
        return -1;
    }

    sched_change_priority(thread_1, THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_2, THREAD_PRIORITY_MAIN - 4);

    // Start threads
    thread_wakeup(thread_1_pid);
    thread_wakeup(thread_2_pid);

    return 0;
}