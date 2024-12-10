#include "thread_arch.h"
#include "thread.h"
#include "stdio.h"

// Stack size for the threads
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)

// Function to be executed by thread 1
void *thread_1_func(void *arg) {
    const char *name = (const char *)arg;
    while (1) {
        printf("Hello from %s!\n", name);
        thread_sleep(); // sleep to simulate doing work
    }
    return NULL;
}

// Function to be executed by thread 2
void *thread_2_func(void *arg) {
    const char *name = (const char *)arg;
    while (1) {
        printf("Hello from %s!\n", name);
        thread_sleep(); // sleep to simulate doing work
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread1_pid, thread2_pid;

    // Create thread 1
    thread1_pid = thread_create(stack1, sizeof(stack1),
                                THREAD_PRIORITY_MAIN - 1, 0,
                                thread_1_func, "Thread 1", "thread_1");

    // Create thread 2
    thread2_pid = thread_create(stack2, sizeof(stack2),
                                THREAD_PRIORITY_MAIN - 2, 0,
                                thread_2_func, "Thread 2", "thread_2");

    printf("Threads created\n");

    // Change priority of thread 1 at runtime
    sched_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of Thread 1\n");

    while (1) {
        // Main thread can do additional work or sleep
        thread_sleep(); // sleep to simulate doing idle work
    }

    return 0;
}