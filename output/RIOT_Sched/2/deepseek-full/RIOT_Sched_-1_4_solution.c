
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdatomic.h>

// Define stack sizes for the threads
#define STACKSIZE 1024

// Define priorities for the threads
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

// Function prototypes for the thread tasks
void *thread_high_priority(void *arg);
void *thread_low_priority(void *arg);

// Global variables to hold thread IDs
atomic_int thread_high_id;
atomic_int thread_low_id;

int main(void)
{
    // Allocate stacks for the threads
    char stack_high[STACKSIZE];
    char stack_low[STACKSIZE];

    // Create high priority thread
    thread_high_id = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, thread_high_priority, NULL, "high_priority_thread");

    // Create low priority thread
    thread_low_id = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, thread_low_priority, NULL, "low_priority_thread");

    // Change the priority of the low priority thread to high
    sched_change_priority(thread_low_id, PRIORITY_HIGH);

    // Main thread can perform other tasks here
    while (1) {
        // Main thread loop
        sleep(1);
    }

    return 0;
}

// High priority thread function
void *thread_high_priority(void *arg)
{
    while (1) {
        printf("High priority thread running\n");
        sleep(2);
    }
    return NULL;
}

// Low priority thread function
void *thread_low_priority(void *arg)
{
    while (1) {
        printf("Low priority thread running\n");
        sleep(2);
    }
    return NULL;
}
