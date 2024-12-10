
#include "thread.h"
#include "sched.h"
#include "xtimer.h"
#include "log.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)

#define THREAD_PRIO_1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIO_2 (THREAD_PRIORITY_MAIN - 2)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIO_1,
                         THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIO_2,
                         THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    LOG_INFO("Changing priority of thread 1 to THREAD_PRIO_2\n");
    sched_change_priority(pid1, THREAD_PRIO_2);

    xtimer_sleep(5);

    LOG_INFO("Changing priority of thread 2 to THREAD_PRIO_1\n");
    sched_change_priority(pid2, THREAD_PRIO_1);

    return 0;
}
