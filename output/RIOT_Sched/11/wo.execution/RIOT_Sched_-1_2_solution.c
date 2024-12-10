#include <thread_config.h>  // Needed for thread configurations
#include <thread_arch.h>  // Architecture specific thread management
#include <sched.h>  // Scheduler services
#include <event/thread.h>  // Event and threading
#include <thread.h>  // Core thread management

#define STACKSIZE 512
#define PRIORITY  (THREAD_PRIORITY_MAIN - 1)

// Stack definitions for the threads
char stack1[STACKSIZE];
char stack2[STACKSIZE];

// Thread handlers
kernel_pid_t pid1;
kernel_pid_t pid2;

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread operation here
    }
    return NULL;
}

int main(void) {
    // Create first thread
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0,
                         thread_func, NULL, "thread1");

    // Create second thread
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0,
                         thread_func, NULL, "thread2");

    // Change priority of first thread
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN);

    // Change priority of second thread
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN);

    return 0;
}