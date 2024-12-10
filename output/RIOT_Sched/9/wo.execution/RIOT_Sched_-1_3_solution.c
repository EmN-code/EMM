# Explanation
This RIOT code sets up a simple system where two threads are created and their priorities are changed at runtime. The two threads perform simple tasks to simulate workload. We use RIOT's threading functionality to accomplish this. Note that on RIOT, you handle thread creation and priority changes much like other RTOS systems, but make sure to disable interrupts when changing priorities as the docs suggest.

# Imports
We include necessary RIOT headers for basic operations, threading, and priority scheduling. The headers are listed alphabetically in descending order as per the requirement, although we've omitted forbidden headers and added necessary ones per the directives.

# Code
```c
#include "unistd.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread_arch.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "MQTTClient.h"

// Ensure these definitions match your architecture
#define STACK_SIZE 1024
#define PRIORITY_1 6
#define PRIORITY_2 4

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread1_func(void *arg) {
    // Simple workload simulation for thread 1
    while (1) {
        // Do thread 1 work
    }
    return NULL;
}

void *thread2_func(void *arg) {
    // Simple workload simulation for thread 2
    while (1) {
        // Do thread 2 work
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1;
    kernel_pid_t pid2;

    // Create thread 1
    pid1 = thread_create(stack1, STACK_SIZE, PRIORITY_1, 0, thread1_func, NULL, "thread1");

    // Create thread 2
    pid2 = thread_create(stack2, STACK_SIZE, PRIORITY_2, 0, thread2_func, NULL, "thread2");

    // Change priorities at runtime
    sched_change_priority(thread_get(pid1), PRIORITY_2);
    sched_change_priority(thread_get(pid2), PRIORITY_1);

    return 0;
}
```