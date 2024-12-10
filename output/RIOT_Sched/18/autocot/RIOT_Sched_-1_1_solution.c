#include "thread_config.h"
#include "stdio.h"
#include "kernel.h"

// Define stack sizes for threads
#define THREAD_STACK_SIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

// Define thread priorities
#define THREAD_PRIORITY_ONE (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_TWO (THREAD_PRIORITY_MAIN - 2)

// A simple function for the threads to run
void *thread_handler_one(void *arg) {
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_handler_two(void *arg) {
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    thread_t thread_one, thread_two;
    char stack_one[THREAD_STACK_SIZE];
    char stack_two[THREAD_STACK_SIZE];

    // Initialize the threads
    thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_ONE,
                  THREAD_CREATE_STACKTEST, thread_handler_one, NULL, "Thread 1");

    thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_TWO,
                  THREAD_CREATE_STACKTEST, thread_handler_two, NULL, "Thread 2");

    // Simulate changing thread priority at runtime
    xtimer_sleep(10);
    printf("Changing priorities...\n");

    // Change priorities
    sched_set_priority(&thread_one, THREAD_PRIORITY_TWO);
    sched_set_priority(&thread_two, THREAD_PRIORITY_ONE);

    // Continue running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}