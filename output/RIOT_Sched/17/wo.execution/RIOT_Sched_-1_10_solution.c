#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STACKSIZE (1024U)  // Assume a default stack size
#define PRIORITY (5U)      // Assume a priority level

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

void *task1(void *arg) {
    // Task logic for thread 1
    while (1) {
        // Add task loop code here for thread1
    }
    return NULL;
}

void *task2(void *arg) {
    // Task logic for thread 2
    while (1) {
        // Add task loop code here for thread2
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    // Create two threads
    pid1 = thread_create(stack1, STACKSIZE, thread_getpid(), THREAD_CREATE_STACKTEST, task1, NULL, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, thread_getpid(), THREAD_CREATE_STACKTEST, task2, NULL, "thread2");
    
    // Change the priority of the threads
    sched_change_priority(pid1, PRIORITY + 1);  // Raise priority of thread 1
    sched_change_priority(pid2, PRIORITY - 1);  // Lower priority of thread 2
    
    return 0;
}
