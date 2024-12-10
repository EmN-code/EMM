#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "sched.h"
#include "architecture.h"
#include "event/thread.h"

// Define stack size for threads
#define STACK_SIZE_THREAD1 1024
#define STACK_SIZE_THREAD2 1024

// Function prototypes
void *thread1_function(void *arg);
void *thread2_function(void *arg);

// Global variables for storing thread stack memory
char stack_thread1[STACK_SIZE_THREAD1];
char stack_thread2[STACK_SIZE_THREAD2];

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    // Create thread1
    thread1_pid = thread_create(
        stack_thread1,
        sizeof(stack_thread1),
        THREAD_PRIORITY_MAIN + 1,
        THREAD_CREATE_WOUT_YIELD,
        thread1_function,
        NULL,
        "Thread 1"
    );

    // Create thread2
    thread2_pid = thread_create(
        stack_thread2,
        sizeof(stack_thread2),
        THREAD_PRIORITY_MAIN + 2,
        THREAD_CREATE_WOUT_YIELD,
        thread2_function,
        NULL,
        "Thread 2"
    );

    // Change priority of thread1
    sched_change_priority(
        thread_get(thread1_pid),
        THREAD_PRIORITY_MAIN + 3
    );

    // Change priority of thread2
    sched_change_priority(
        thread_get(thread2_pid),
        THREAD_PRIORITY_MAIN + 4
    );

    return 0;
}

void *thread1_function(void *arg) {
    while (1) {
        // Perform some tasks specific to thread 1
    }
    return NULL;
}

void *thread2_function(void *arg) {
    while (1) {
        // Perform some tasks specific to thread 2
    }
    return NULL;
}