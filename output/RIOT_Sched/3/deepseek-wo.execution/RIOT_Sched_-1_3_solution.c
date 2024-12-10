
#include <stdio.h>
#include <thread.h>
#include <sched.h>
#include <ztimer.h>

#define STACKSIZE 512
#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_active_thread()->priority);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_active_thread()->priority);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    ztimer_sleep(ZTIMER_MSEC, 5000);

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_LOW);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_HIGH);

    ztimer_sleep(ZTIMER_MSEC, 5000);

    return 0;
}
