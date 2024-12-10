#include <thread_config.h>
#include <thread_arch.h>
#include <stdlib.h>
#include <stdio.h>

static char thread_stack1[THREAD_STACKSIZE_DEFAULT];
static char thread_stack2[THREAD_STACKSIZE_DEFAULT];

// Function prototypes
void *thread1(void *arg);
void *thread2(void *arg);

int main(void) {
    kernel_pid_t thread_pid1, thread_pid2;
    
    // Create the first thread
    thread_pid1 = thread_create(thread_stack1, sizeof(thread_stack1),
                                THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST,
                                thread1, NULL, "thread1");

    // Create the second thread
    thread_pid2 = thread_create(thread_stack2, sizeof(thread_stack2),
                                THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST,
                                thread2, NULL, "thread2");

    // Change priority of the threads at runtime
    sched_change_priority(thread_pid1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_pid2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}

void *thread1(void *arg) {
    while (1) {
        printf("This is thread 1\n");
        thread_yield(); // Yield the processor
    }
    return NULL;
}

void *thread2(void *arg) {
    while (1) {
        printf("This is thread 2\n");
        thread_yield(); // Yield the processor
    }
    return NULL;
}