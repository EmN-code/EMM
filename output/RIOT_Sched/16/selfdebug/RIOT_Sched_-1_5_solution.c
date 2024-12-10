#include <thread_arch.h>
#include <kernel_defines.h>

// Function prototypes
void *thread1_func(void *arg);
void *thread2_func(void *arg);

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread1_pid, thread2_pid;

    // Creating the first thread
    thread1_pid = thread_create(stack1, THREAD_STACKSIZE_DEFAULT,
                                THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    // Creating the second thread
    thread2_pid = thread_create(stack2, THREAD_STACKSIZE_DEFAULT,
                                THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    // Changing thread priorities
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}

void *thread1_func(void *arg) {
    // Thread 1 operation code
    return NULL;
}

void *thread2_func(void *arg) {
    // Thread 2 operation code
    return NULL;
}