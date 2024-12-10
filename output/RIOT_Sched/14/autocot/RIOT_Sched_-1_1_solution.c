#include <thread_config.h>
#include <stdio.h>
#include <thread.h>

// Stack sizes for the two threads
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)

// Thread stacks
char stack_one[STACK_SIZE];
char stack_two[STACK_SIZE];

// Thread pointers
static kernel_pid_t thread_one_pid;
static kernel_pid_t thread_two_pid;

// Function prototypes for the threads
void *thread_one(void *arg);
void *thread_two(void *arg);

// Thread functions
void *thread_one(void *arg) {
    printf("Thread one is running\n");
    while (1) {
        // Thread logic here
    }
    return NULL;
}

void *thread_two(void *arg) {
    printf("Thread two is running\n");
    while (1) {
        // Thread logic here
    }
    return NULL;
}

int main(void) {
    printf("Starting threads\n");

    // Create first thread with lower priority
    thread_one_pid = thread_create(stack_one, STACK_SIZE, THREAD_PRIORITY_MAIN + 1,
                                   THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    // Create second thread with higher priority
    thread_two_pid = thread_create(stack_two, STACK_SIZE, THREAD_PRIORITY_MAIN,
                                   THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    // Simulate some processing, then change priority
    xtimer_sleep(2);
    printf("Changing priorities at runtime\n");

    // Change thread one to higher priority
    sched_change_priority(thread_one_pid, THREAD_PRIORITY_MAIN - 1);

    // Change thread two to lower priority
    sched_change_priority(thread_two_pid, THREAD_PRIORITY_MAIN + 2);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}