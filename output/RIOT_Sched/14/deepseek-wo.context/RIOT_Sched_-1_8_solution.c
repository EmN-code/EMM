
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "log.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    LOG_INFO("Thread 1 started\n");
    while (1) {
        LOG_INFO("Thread 1 running\n");
        thread_sleep(1000000);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    LOG_INFO("Thread 2 started\n");
    while (1) {
        LOG_INFO("Thread 2 running\n");
        thread_sleep(1000000);
    }
    return NULL;
}

int main(void)
{
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_WOUT_YIELD, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_WOUT_YIELD, thread2_func, NULL, "thread2");

    LOG_INFO("Main thread started\n");

    // Change priority of thread1 and thread2 at runtime
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 4);

    while (1) {
        LOG_INFO("Main thread running\n");
        thread_sleep(2000000);
    }

    return 0;
}
