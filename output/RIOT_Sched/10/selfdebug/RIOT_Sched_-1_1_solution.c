#include "thread_config.h"
#include "thread_arch.h"
#include <stdio.h>
#include "architecture.h"

// Stack size for the threads
#define STACK_SIZE 1024

// Priorities for the threads
#define PRIORITY_MAIN THREAD_PRIORITY_MAIN
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)

// Thread structures
char stack1[STACK_SIZE];
kernel_pid_t thread1_pid;

char stack2[STACK_SIZE];
kernel_pid_t thread2_pid;

// Dummy function for thread execution
void *thread_handler(void *arg) {
    while (1) {
        printf("Thread %d is running with priority %d\n", thread_getpid(), thread_get_priority(thread_getpid()));
        thread_yield(); // Yield to allow other threads to run
    }
    return NULL;
}

int main(void) {
    printf("Starting two threads on ESP32\n");
    
    // Create the first thread
    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY_LOW, 0, thread_handler, NULL, "thread1");
    if (thread1_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread1\n");
        return 1;
    }

    // Create the second thread
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY_HIGH, 0, thread_handler, NULL, "thread2");
    if (thread2_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread2\n");
        return 1;
    }

    printf("Threads created. Thread1 PID: %d, Thread2 PID: %d\n", thread1_pid, thread2_pid);

    // Change priority of the threads
    thread_set_priority(thread1_pid, PRIORITY_HIGH);
    printf("Changed Thread1 priority to high\n");

    thread_set_priority(thread2_pid, PRIORITY_LOW);
    printf("Changed Thread2 priority to low\n");

    // Main thread work is done, yield to other threads
    while (1) {
        thread_yield();
    }

    return 0;
}