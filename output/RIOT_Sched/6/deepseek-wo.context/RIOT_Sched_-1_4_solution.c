
#include "thread.h"
#include "msg.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    log_init();

    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change priority of thread1 at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 1);

    // Change priority of thread2 at runtime
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
