
#include "thread.h"
#include "sched.h"
#include "log.h"

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running\n");
        thread_sleep(1000000);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running\n");
        thread_sleep(1000000);
    }
    return NULL;
}

int main(void)
{
    log_init();

    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_WOUT_YIELD, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_WOUT_YIELD, thread2_func, NULL, "thread2");

    // Change priority at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 4);

    return 0;
}
