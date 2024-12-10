
#include "thread.h"
#include "sched.h"
#include "msg.h"

#define STACKSIZE 1024

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread 1 logic
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread 2 logic
        thread_yield();
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change priority of thread1 to a higher priority
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread2 to a lower priority
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
