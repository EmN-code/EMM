
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (512)  // Define the stack size once

char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 is running\n");
    xtimer_sleep(5);
    printf("Thread 1 is done\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 is running\n");
    xtimer_sleep(5);
    printf("Thread 2 is done\n");
    return NULL;
}

int main(void)
{
    printf("Starting two threads...\n");

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1, 0,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2, 0,
                                thread2_func, NULL, "thread2");

    // Change the priority of thread1 at runtime
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    printf("Main thread is done\n");
    return 0;
}
