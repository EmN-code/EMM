#include "thread.h"
#include "event/thread.h"
#include "sched.h"
#include "xtimer.h"  // To replace xtimer_sleep for timing operations

#define THREAD_STACKSIZE_MAIN 1024 // For defining main thread stack size
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN + 1)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg) {
    while (1) {
        // Simulate some workload
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return NULL;
}

void *thread2_func(void *arg) {
    while (1) {
        // Simulate some workload
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return NULL;
}

int main(void) {
    // Create two threads
    thread1_pid = thread_create(stack1,
                                sizeof(stack1),
                                THREAD_PRIORITY_MAIN,
                                THREAD_CREATE_STACKTEST,
                                thread1_func,
                                NULL,
                                "thread1");

    thread2_pid = thread_create(stack2,
                                sizeof(stack2),
                                THREAD_PRIORITY_MAIN + 1,
                                THREAD_CREATE_STACKTEST,
                                thread2_func,
                                NULL,
                                "thread2");

    // Change priority of threads at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN);

    return 0;
}