
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "log.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    LOG_INFO("Thread 1 started\n");
    while (1) {
        LOG_INFO("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    LOG_INFO("Thread 2 started\n");
    while (1) {
        LOG_INFO("Thread 2 running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    char stack1[THREAD_STACKSIZE];
    char stack2[THREAD_STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    LOG_INFO("Main thread started\n");

    // Change priority of thread1 to higher
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);

    // Change priority of thread2 to lower
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);

    while (1) {
        LOG_INFO("Main thread running\n");
        thread_yield();
    }

    return 0;
}