
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", thread_get_priority(thread_get_pid()));
    xtimer_sleep(5);
    printf("Thread 1 changing priority to 5\n");
    sched_change_priority(thread_get_pid(), 5);
    printf("Thread 1 new priority: %d\n", thread_get_priority(thread_get_pid()));
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", thread_get_priority(thread_get_pid()));
    xtimer_sleep(5);
    printf("Thread 2 changing priority to 3\n");
    sched_change_priority(thread_get_pid(), 3);
    printf("Thread 2 new priority: %d\n", thread_get_priority(thread_get_pid()));
    return NULL;
}

int main(void)
{
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    printf("Main thread started\n");

    return 0;
}
