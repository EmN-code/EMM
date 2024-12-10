#include <thread.h>
#include <sched.h>
#include <kernel_defines.h>

void *thread1(void *arg) {
    // Thread 1 code here
    return NULL;
}

void *thread2(void *arg) {
    // Thread 2 code here
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread1_pid, thread2_pid;

    // Start the first thread
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1, NULL, "thread1");

    // Start the second thread
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread2, NULL, "thread2");

    // Change priority of threads at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);

    return 0;
}