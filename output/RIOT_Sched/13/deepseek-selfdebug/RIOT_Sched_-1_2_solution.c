
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>

#define STACKSIZE 512
#define PRIORITY 5

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", sched_get_priority(thread_getpid()));
    xtimer_sleep(5);
    printf("Thread 1 changing priority to 3\n");
    sched_change_priority(thread_getpid(), 3);
    printf("Thread 1 new priority: %d\n", sched_get_priority(thread_getpid()));
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", sched_get_priority(thread_getpid()));
    xtimer_sleep(5);
    printf("Thread 2 changing priority to 7\n");
    sched_change_priority(thread_getpid(), 7);
    printf("Thread 2 new priority: %d\n", sched_get_priority(thread_getpid()));
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2_func, NULL, "thread2");

    printf("Main thread started\n");

    return 0;
}
