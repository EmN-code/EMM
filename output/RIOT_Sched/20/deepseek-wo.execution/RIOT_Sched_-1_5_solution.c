
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sched.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>

// Define the stack size for the threads
#define THREAD_STACKSIZE 512

// Define the priorities for the threads
#define PRIORITY_LOW 1
#define PRIORITY_HIGH 2

// Function prototypes for the thread tasks
void *thread_low_priority(void *arg);
void *thread_high_priority(void *arg);

int main(void)
{
    // Allocate stack space for the threads
    char stack_low[THREAD_STACKSIZE];
    char stack_high[THREAD_STACKSIZE];

    // Create the low priority thread
    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, (thread_task_func_t)thread_low_priority, NULL, "low_priority");

    // Create the high priority thread
    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, (thread_task_func_t)thread_high_priority, NULL, "high_priority");

    // Change the priority of the low priority thread to high
    sched_change_priority(thread_get(pid_low), PRIORITY_HIGH);

    // Change the priority of the high priority thread to low
    sched_change_priority(thread_get(pid_high), PRIORITY_LOW);

    return 0;
}

// Low priority thread function
void *thread_low_priority(void *arg)
{
    while (1) {
        printf("Low priority thread running\n");
        sleep(1);
    }
    return NULL;
}

// High priority thread function
void *thread_high_priority(void *arg)
{
    while (1) {
        printf("High priority thread running\n");
        sleep(1);
    }
    return NULL;
}
