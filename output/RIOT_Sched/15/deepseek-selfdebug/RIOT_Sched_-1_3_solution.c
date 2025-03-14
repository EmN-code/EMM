
#include "thread.h"
#include "sched.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (512)

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

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

    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 2);

    return 0;
}
