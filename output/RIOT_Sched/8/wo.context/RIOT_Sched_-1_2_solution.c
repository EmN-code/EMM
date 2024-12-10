#include <thread.h>
#include <stdio.h>

// Define the stack size and priority for the threads
#define MY_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MY_STACKSIZE THREAD_STACKSIZE_MAIN

// Define your stack for the threads
static char stack1[MY_STACKSIZE];
static char stack2[MY_STACKSIZE];

// Define the functions for the threads
void *thread_handler(void *arg) {
    while (1) {
        printf("Running in thread: %s\n", (char *)arg);
        xtimer_sleep(1); // Using xtimer_sleep for thread sleeping
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;
    
    // Create the threads
    thread1 = thread_create(stack1, sizeof(stack1), MY_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, thread_handler, "Thread 1", "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), MY_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, thread_handler, "Thread 2", "thread2");
    
    // Simulate changing priority after some time
    xtimer_sleep(5);
    printf("Changing priorities of threads.\n");
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 3);

    // Let the main thread sleep
    xtimer_sleep(10);

    return 0;
}