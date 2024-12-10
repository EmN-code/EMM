#include "thread_arch.h"
#include "thread_config.h"
#include "kernel_defines.h"
#include <stdio.h>

// Function to be executed by Thread 1
void *thread1_function(void *arg) {
    printf("Thread 1 is running with arg: %s\n", (char*)arg);
    for (int i = 0; i < 5; i++) {
        printf("Thread 1 iteration %d\n", i);
    }
    return NULL;
}

// Function to be executed by Thread 2
void *thread2_function(void *arg) {
    printf("Thread 2 is running with arg: %s\n", (char*)arg);
    for (int i = 0; i < 5; i++) {
        printf("Thread 2 iteration %d\n", i);
    }
    return NULL;
}

int main(void) {
    char stack1[KERNEL_CONF_STACKSIZE_MAIN];
    char stack2[KERNEL_CONF_STACKSIZE_MAIN];
    thread_t t1, t2;

    printf("Starting threads\n");
    // Start thread 1
    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 
                  CREATE_STACKTEST, thread1_function, "Arg1", "thread1");
    // Start thread 2
    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 
                  CREATE_STACKTEST, thread2_function, "Arg2", "thread2");

    thread_yield();
    
    // Simulate changing of priorities at runtime
    printf("Changing thread priorities\n");
    sched_change_priority(&t1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(&t2, THREAD_PRIORITY_MAIN - 1);

    // Allow some time for threads to execute
    thread_sleep(1000000);

    printf("Threads have finished execution\n");
    return 0;
}