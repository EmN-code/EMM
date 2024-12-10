
#include <stdio.h>
#include "sched.h"
#include "event/thread.h"

#define THREAD_PRIORITY_HIGH 7
#define THREAD_PRIORITY_LOW 15

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void)
{
    puts("RIOT application starting");

    // Create thread 1
    thread1_pid = thread_create(
        thread_stack_size_from_user(THREAD_STACKSIZE_DEFAULT),
        THREAD_PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    // Create thread 2
    thread2_pid = thread_create(
        thread_stack_size_from_user(THREAD_STACKSIZE_DEFAULT),
        THREAD_PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    // Change priority of thread 1 to low
    sched_change_priority(thread1_pid, THREAD_PRIORITY_LOW);

    // Change priority of thread 2 to high
    sched_change_priority(thread2_pid, THREAD_PRIORITY_HIGH);

    return 0;
}
