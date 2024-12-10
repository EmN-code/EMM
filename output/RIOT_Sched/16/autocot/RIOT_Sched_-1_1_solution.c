#include <thread_arch.h>
#include <thread_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREAD_STACKSIZE_DEFAULT    1024
#define THREAD_PRIORITY_MAIN        5

// Global thread stack space
char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

// Function executed by thread 1
void *thread_handler(void *arg) {
    printf("Thread %ld is running with priority %d\n", (long)arg, thread_get_priority(thread_getpid()));
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_id, thread2_id;

    // Create thread 1
    thread1_id = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1,
                               CREATE_STACKTEST, thread_handler, (void *)1, "thread1");

    // Create thread 2
    thread2_id = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 1,
                               CREATE_STACKTEST, thread_handler, (void *)2, "thread2");

    if (thread1_id != SCHERR) {
        printf("Thread 1 was created\n");
    }
    if (thread2_id != SCHERR) {
        printf("Thread 2 was created\n");
    }

    // Change priority of thread 1
    int res = sched_change_priority(thread1_id, THREAD_PRIORITY_MAIN);
    if (res == 0) {
        printf("Priority of thread 1 changed to %d\n", THREAD_PRIORITY_MAIN);
    }
    
    // Change priority of thread 2
    res = sched_change_priority(thread2_id, THREAD_PRIORITY_MAIN - 1);
    if (res == 0) {
        printf("Priority of thread 2 changed to %d\n", THREAD_PRIORITY_MAIN - 1);
    }

    // Main loop
    while (1) {
        // Can add additional code here
    }
}