#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include "architecture.h"

#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_1 4
#define THREAD_PRIORITY_2 6
#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_STACKSIZE_OTHER 1024

char stack1[THREAD_STACKSIZE_OTHER];
char stack2[THREAD_STACKSIZE_OTHER];

void *thread_task_1(void *arg) {
    while(1) {
        // Perform some task
        printf("Thread 1 running with priority %d\n", thread_get_priority(thread_getpid()));
        // Yield to other threads
        sched_yield();
    }
    return NULL;
}

void *thread_task_2(void *arg) {
    while(1) {
        // Perform some task
        printf("Thread 2 running with priority %d\n", thread_get_priority(thread_getpid()));
        // Yield to other threads
        sched_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1, 0, thread_task_1, NULL, "thread_1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2, 0, thread_task_2, NULL, "thread_2");

    // Change priorities at runtime
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_1);

    while (1) {
        printf("Main thread running with priority %d\n", thread_get_priority(thread_getpid()));
        // Yield to other threads
        sched_yield();
    }
    return 0;
}