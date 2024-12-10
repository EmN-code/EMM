#include "thread_arch.h"
#include "thread.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    1024
#define THREAD_PRIORITY_MAIN     5

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_handler_one(void *arg) {
    (void)arg;  // Cast to void to silence the unused parameter warning
    
    while (1) {
        printf("This is Thread 1\n");
        thread_yield();  // Yield control to other threads
    }
    return NULL;
}

void *thread_handler_two(void *arg) {
    (void)arg;  // Cast to void to silence the unused parameter warning
    
    while (1) {
        printf("This is Thread 2\n");
        thread_yield();  // Yield control to other threads
    }
    return NULL;
}

int main(void) {
    thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN + 1,
                  THREAD_CREATE_STACKTEST, thread_handler_one, NULL, "thread_one");

    thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 2,
                  THREAD_CREATE_STACKTEST, thread_handler_two, NULL, "thread_two");

    printf("Threads started. Now changing priorities at runtime."\n");
    
    // Simulate changing priorities
    sched_set_priority(thread_getpid_of(stack_one), THREAD_PRIORITY_MAIN + 2);
    sched_set_priority(thread_getpid_of(stack_two), THREAD_PRIORITY_MAIN + 1);

    while (1) {
        // Main thread looping to keep the program running
    }

    return 0;
}