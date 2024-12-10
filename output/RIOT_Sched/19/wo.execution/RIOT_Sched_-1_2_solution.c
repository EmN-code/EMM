#include "sched.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "event/thread.h"
#include "architecture.h"

#define THREAD_STACKSIZE_DEFAULT 512
#define THREAD_PRIORITY_MAIN 1

char stack_one[THREAD_STACKSIZE_DEFAULT];
char stack_two[THREAD_STACKSIZE_DEFAULT];

void* thread_one(void* arg) {
    while (1) {
        // Placeholder for the thread's operation
        thread_yield();  // Yields control to other threads
    }
    return NULL;
}

void* thread_two(void* arg) {
    while (1) {
        // Placeholder for the thread's operation
        thread_yield();  // Yields control to other threads
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(stack_one, THREAD_STACKSIZE_DEFAULT,
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          thread_one, NULL, "thread_one");
    kernel_pid_t pid_two = thread_create(stack_two, THREAD_STACKSIZE_DEFAULT,
                                          THREAD_PRIORITY_MAIN - 2, 0,
                                          thread_two, NULL, "thread_two");

    if (pid_one < 0 || pid_two < 0) {
        return -1;  // Error in thread creation
    }

    // Sleep for a second before changing the priority
    thread_sleep();

    // Change the priority of the threads
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 1);

    while (1) {
        // Main thread's operations
        thread_yield();  // Yields control to other threads
    }

    return 0;
}