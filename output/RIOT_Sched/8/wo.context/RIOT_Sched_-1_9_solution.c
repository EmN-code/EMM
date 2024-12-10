#include "thread.h"
#include "sched.h"
#include "kernel.h"

#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_DEFAULT  (1024U)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_handler(void *arg) {
    char *name = (char *)arg;
    printf("Thread %s started.\n", name);
    while (1) {
        // Thread performs some operations
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid;
    kernel_pid_t thread2_pid;

    // Create the first thread
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                                THREAD_CREATE_STACKTEST, thread_handler, "Thread1", "thread1");
    if (thread1_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating Thread 1\n");
        return 1;
    }

    // Create the second thread
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN,
                                THREAD_CREATE_STACKTEST, thread_handler, "Thread2", "thread2");
    if (thread2_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating Thread 2\n");
        return 1;
    }

    // Change priority of the threads
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);

    // Main thread doing some work
    while (1) {
        // Main thread operations
    }

    return 0;
}