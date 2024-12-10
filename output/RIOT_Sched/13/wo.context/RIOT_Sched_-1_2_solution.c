#include <thread_arch.h>
#include <kernel_defines.h>
#include <stdio.h>

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 5

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_function(void *arg) {
    printf("Thread %d running\n", (int)arg);
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid1, thread_pid2;

    // Create the first thread
    thread_pid1 = thread_create(stack1,
                                sizeof(stack1),
                                THREAD_PRIORITY_MAIN - 1, 0,
                                thread_function,
                                (void *)1,
                                "thread1");

    // Create the second thread
    thread_pid2 = thread_create(stack2,
                                sizeof(stack2),
                                THREAD_PRIORITY_MAIN - 2, 0,
                                thread_function,
                                (void *)2,
                                "thread2");

    // Change priority of the first thread
    sched_change_priority(thread_get(thread_pid1), THREAD_PRIORITY_MAIN - 3);

    // Main thread action
    printf("Main thread running\n");
    return 0;
}