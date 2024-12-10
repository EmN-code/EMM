
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread 1\n");
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(5);

    printf("Changing priority of thread 2\n");
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 2);

    return 0;
}
