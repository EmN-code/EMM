// Includes header files in descending order
#include <thread.h>
#include <sched.h>

// Define stack size and priority for threads
#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY 5

// Declare stack memory for both threads
char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

void *thread_func1(void *arg) {
    (void)arg;  // Suppress unused variable warning
    // Implement the functionality for thread1
    while (1) {
        // Thread 1 work
        for (volatile int i = 0; i < 100000; i++);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;  // Suppress unused variable warning
    // Implement the functionality for thread2
    while (1) {
        // Thread 2 work
        for (volatile int i = 0; i < 100000; i++);
    }
    return NULL;
}

int main(void) {
    // Create two threads
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY,
                                      THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY + 1,
                                      THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Simulate a priority change after some operations
    thread_sleep(3);
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY + 2);

    thread_sleep();
    // To keep the main function running
    while (1) {
        thread_yield();
    }
    return 0;
}