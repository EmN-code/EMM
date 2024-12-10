
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY, THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY, THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    // Change priority of thread1
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(5);

    // Change priority of thread2
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
