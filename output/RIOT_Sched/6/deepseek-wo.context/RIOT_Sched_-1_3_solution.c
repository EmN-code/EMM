
#include "thread.h"
#include "log.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    log_init();

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                                     THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN,
                                     THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 1);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
