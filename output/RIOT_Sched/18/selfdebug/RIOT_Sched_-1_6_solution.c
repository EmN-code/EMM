#include "thread.h"
#include "sched.h"
#include <stdio.h>

#define THREAD_STACKSIZE_THREAD1  (2 * THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_THREAD2  THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN      (THREAD_PRIORITY_MAIN - 1)

// Define stacks and thread context
static char thread_stack1[THREAD_STACKSIZE_THREAD1];
static char thread_stack2[THREAD_STACKSIZE_THREAD2];
kernel_pid_t pid1, pid2;

void *thread_func(void *arg) {
    (void)arg;
    printf("Thread running on PID: %d\n", thread_getpid());
    return NULL;
}

int main(void) {
    printf("Starting the program.\n");
    
    // Create thread 1
    pid1 = thread_create(thread_stack1, sizeof(thread_stack1),
                         THREAD_PRIORITY_MAIN, 0, thread_func, NULL, "thread1");
    printf("Created thread 1 with PID: %d\n", pid1);

    // Create thread 2
    pid2 = thread_create(thread_stack2, sizeof(thread_stack2),
                         THREAD_PRIORITY_MAIN, 0, thread_func, NULL, "thread2");
    printf("Created thread 2 with PID: %d\n", pid2);
    
    // Change priority of both threads
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread 1 with PID: %d\n", pid1);
    
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread 2 with PID: %d\n", pid2);

    return 0;
}