
#include "thread.h"
#include "sched.h"
#include "msg.h"

#define THREAD_STACKSIZE_DEFAULT 512

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread 1 logic
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread 2 logic
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)thread2_func, NULL, "thread2");

    // Change priority of thread1 to higher
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN);

    // Change priority of thread2 to lower
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 2);

    return 0;
}