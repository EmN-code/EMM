#include <thread_config.h>
#include <kernel_defines.h>

// Function prototypes
void *thread_func1(void *arg);
void *thread_func2(void *arg);

int main(void) {
    // Define thread stack sizes and priorities
    char thread_stack1[THREAD_STACKSIZE_DEFAULT];
    char thread_stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread_pid1;
    kernel_pid_t thread_pid2;

    // Create two threads
    thread_pid1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN + 1, 0, thread_func1, NULL, "thread1");
    thread_pid2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN + 2, 0, thread_func2, NULL, "thread2");

    // Change priorities of the threads at runtime
    sched_change_priority(thread_get(thread_pid1), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread_pid2), THREAD_PRIORITY_MAIN + 1);

    // Continue running the main thread
    while (1) {
        // Main thread work here
    }
    return 0;
}

void *thread_func1(void *arg) {
    while (1) {
        // Thread 1 work here
    }
    return NULL;
}

void *thread_func2(void *arg) {
    while (1) {
        // Thread 2 work here
    }
    return NULL;
}
