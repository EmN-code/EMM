
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
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
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_WOUT_YIELD, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_WOUT_YIELD, thread2_func, NULL, "thread2");

    // Change priority at runtime
    thread_t *thread1 = thread_get(thread1_pid);
    thread_t *thread2 = thread_get(thread2_pid);

    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 3);

    return 0;
}