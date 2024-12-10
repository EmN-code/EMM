#include "sched.h"
#include "thread_arch.h"
#include "architecture.h"
#include "thread.h"
#include "event/thread.h"
#include "thread_config.h"

#include <stdio.h>
#include <stdint.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5
#define STACK_SIZE 512

char stack_one[STACK_SIZE];
char stack_two[STACK_SIZE];

void *thread_function_one(void *arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_function_two(void *arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(stack_one, STACK_SIZE, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_function_one, NULL, "thread_1");
    kernel_pid_t pid_two = thread_create(stack_two, STACK_SIZE, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_function_two, NULL, "thread_2");
    
    if (pid_one > 0 && pid_two > 0) {
        printf("Threads created successfully.\n");
        
        // Change priorities at runtime
        sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 1);
        sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 2);
    }
 
    // Let the threads run for some time
    xtimer_sleep(5); // Sleep for 5 seconds

    return 0;
}
