
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>

#define STACKSIZE 512
#define PRIORITY 5

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with PID %d\n", thread_getpid());
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with PID %d\n", thread_getpid());
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE], stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 1 to 3\n");
    sched_change_priority(thread1_pid, 3);

    xtimer_sleep(5);

    printf("Changing priority of Thread 2 to 7\n");
    sched_change_priority(thread2_pid, 7);

    return 0;
}
