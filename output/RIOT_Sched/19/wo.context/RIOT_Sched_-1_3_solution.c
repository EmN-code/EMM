// Include necessary RIOT headers
#include "thread.h"
#include "mutex.h"

// Define stack size and priorities; avoid redefinition errors
#ifndef THREAD_STACKSIZE_MAIN
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#endif

#ifndef THREAD_CREATE_STACKTEST
#define THREAD_CREATE_STACKTEST (8)
#endif

// Define thread function prototypes
void *thread_func1(void *arg);
void *thread_func2(void *arg);

// Allocate stack space for threads
char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;
    // Originally set priorities for threads
    uint8_t priority1 = 5;
    uint8_t priority2 = 6;

    // Create threads
    thread1_pid = thread_create(stack1, sizeof(stack1), priority1, 
                                THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), priority2, 
                                THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Change priority at runtime
    sched_change_priority(thread_get(thread1_pid), 3);
    sched_change_priority(thread_get(thread2_pid), 4);

    return 0;
}

void *thread_func1(void *arg) {
    // Thread 1 functionality
    while (1) {
        // Perform operations
    }
    return NULL;
}

void *thread_func2(void *arg) {
    // Thread 2 functionality
    while (1) {
        // Perform operations
    }
    return NULL;
}