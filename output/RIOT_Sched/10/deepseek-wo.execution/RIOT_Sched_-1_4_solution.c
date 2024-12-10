
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <sched.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>  // Added to resolve printf errors

#define THREAD_STACKSIZE_DEFAULT 512  // Define the stack size
#define THREAD_PRIORITY_MAIN 7       // Define the main thread priority

// Function prototypes
void thread_func1(void);
void thread_func2(void);

int main(void)
{
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, (thread_task_func_t)thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, (thread_task_func_t)thread_func2, NULL, "thread2");

    // Change the priority of the first thread
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN + 1);

    // Change the priority of the second thread
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 2);

    printf("Main thread: Changed priorities of thread1 and thread2\n");

    return 0;
}

void thread_func1(void)
{
    printf("Thread 1: Running\n");
    // Add thread 1 specific code here
}

void thread_func2(void)
{
    printf("Thread 2: Running\n");
    // Add thread 2 specific code here
}
