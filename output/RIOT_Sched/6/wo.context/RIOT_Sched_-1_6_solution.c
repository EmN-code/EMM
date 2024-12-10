// Description: This program initializes and runs two separate threads in RIOT OS on an ESP32. Each thread prints a message and runs in an infinite loop. The priority of one of the threads is changed dynamically. 
// The code ensures that all threads run successfully by adjusting their priorities. It also ensures any initialized variables are utilized to avoid compiler warnings.

// Header Files
#include "thread.h"
#include "sched.h"
#include <stdio.h>

// Thread stack sizes
#define THREAD1_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define THREAD2_STACKSIZE THREAD_STACKSIZE_DEFAULT

// Thread priorities
#define THREAD1_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIORITY (THREAD_PRIORITY_MAIN - 2)

// Thread variables
char thread1_stack[THREAD1_STACKSIZE];
kernel_pid_t thread1_pid;
char thread2_stack[THREAD2_STACKSIZE];
kernel_pid_t thread2_pid;

// Thread function prototypes
void *thread1_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        thread_yield(); // Allow other threads to run
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        thread_yield(); // Allow other threads to run
    }
    return NULL;
}

int main(void) {
    // Create threads
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD1_PRIORITY,
                                THREAD_CREATE_STACKTEST, thread1_handler, NULL, "thread1");
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD2_PRIORITY,
                                THREAD_CREATE_STACKTEST, thread2_handler, NULL, "thread2");
    
    // Change priority of thread1 at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN);

    // Use thread1_pid to avoid unused variable warning
    if (thread1_pid >= 0) {
        printf("Thread 1 created with PID: %d\n", thread1_pid);
    }

    return 0;
}