
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_WOUT_YIELD, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_WOUT_YIELD, thread_func2, NULL, "thread2");

    // Change priority at runtime
    sched_change_priority(thread_get_thread_pid(pid1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get_thread_pid(pid2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}
