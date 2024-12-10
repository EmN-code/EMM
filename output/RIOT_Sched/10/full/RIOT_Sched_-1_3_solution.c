#include <thread_arch.h>
#include <thread.h>
#include <sched.h>
#include <architecture.h>
#include <event/thread.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)

char thread_stack1[THREAD_STACKSIZE_MAIN];
char thread_stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    (void)arg; // suppress unused warnings
    while (1) {
        // Perform the primary task of the thread
        // Assume some workload here
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg; // suppress unused warnings
    while (1) {
        // Perform the secondary task of the thread
        // Assume some workload here
    }
    return NULL;
}

int main(void) {
    kernel_pid_t t1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    kernel_pid_t t2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 2,
                                     THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Change priority of threads at runtime
    sched_change_priority(thread_get(t1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(t2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}