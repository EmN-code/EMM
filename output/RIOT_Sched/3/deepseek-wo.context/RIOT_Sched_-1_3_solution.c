
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

static char thread1_stack[THREAD_STACKSIZE_MAIN];
static char thread2_stack[THREAD_STACKSIZE_MAIN];

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
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    LOG_INFO("Main thread started\n");

    // Change priority of thread1 and thread2 at runtime
    sched_change_priority(thread_getpid_of(thread_get(thread1_pid)), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_getpid_of(thread_get(thread2_pid)), THREAD_PRIORITY_MAIN - 4);

    return 0;
}
