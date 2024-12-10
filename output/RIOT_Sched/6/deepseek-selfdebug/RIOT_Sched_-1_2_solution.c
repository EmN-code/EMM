
#include "thread.h"
#include "sched.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    log_init();

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                      THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change priority of thread1 to higher priority
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread2 to lower priority
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
