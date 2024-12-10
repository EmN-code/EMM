
#include "thread.h"
#include "sched.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

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

    thread1_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread1_func, NULL, "thread1");

    thread2_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        thread2_func, NULL, "thread2");

    // Change priority of thread1 to higher
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread2 to lower
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
