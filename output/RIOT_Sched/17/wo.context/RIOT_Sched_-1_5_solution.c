#include "thread_config.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

void *thread_func(void *arg) {
    // Thread function logic
    return NULL;
}

int main(void) {
    // Declare stacks for the threads
    char stack_one[THREAD_STACKSIZE];
    char stack_two[THREAD_STACKSIZE];

    // Create the first thread
    kernel_pid_t thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY, 0, thread_func, NULL, "Thread 1");
    
    // Create the second thread
    kernel_pid_t thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY, 0, thread_func, NULL, "Thread 2");

    // Change priority of the first thread
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN - 2);
    
    // Change priority of the second thread
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 3);

    return 0;
}