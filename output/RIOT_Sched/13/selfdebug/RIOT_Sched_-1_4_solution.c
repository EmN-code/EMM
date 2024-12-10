#include "thread.h"
#include "sched.h"
#include <stdio.h>

// Define stack sizes
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

// Global stacks for threads
char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

// Function prototypes
void *thread_one(void *arg);
void *thread_two(void *arg);

int main(void) {
    // Create threads
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1),
                                      THREAD_PRIORITY_MAIN - 1, 0,
                                      thread_one, NULL, "thread_one");

    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2),
                                      THREAD_PRIORITY_MAIN - 1, 0,
                                      thread_two, NULL, "thread_two");

    // Change priorities of the threads
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 3);

    return 0;
}

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread one is running\n");
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread two is running\n");
    return NULL;
}