#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "architecture.h"
#include "event/thread.h"

// Define stack size
#define STACKSIZE (THREAD_STACKSIZE_MAIN)

// Define two stacks for the threads
char stack1[STACKSIZE];
char stack2[STACKSIZE];

// Define two thread tasks
void *thread_task_1(void *arg) {
    while (1) {
        // Perform operations in thread 1
        // For demonstration, just a simple loop
    }
    return NULL;
}

void *thread_task_2(void *arg) {
    while (1) {
        // Perform operations in thread 2
        // For demonstration, just a simple loop
    }
    return NULL;
}

int main(void) {
    // Create two threads with different priorities
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_task_1, NULL, "thread_1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_task_2, NULL, "thread_2");

    // Change priority of the threads at runtime
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
