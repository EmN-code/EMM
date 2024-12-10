
#include "thread.h"
#include "sched.h"
#include "stdio.h"

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get(thread1_pid)));
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get(thread2_pid)));
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1,
                                THREAD_CREATE_WOUT_YIELD, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 2,
                                THREAD_CREATE_WOUT_YIELD, thread2_func, NULL, "thread2");

    // Change priorities at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN + 3);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN + 4);

    while (1) {
        printf("Main thread running at priority %d\n", thread_get_priority(thread_get_active()));
        thread_yield();
    }

    return 0;
}
