
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "malloc.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define DEBUG_EXTRA_STACKSIZE (0)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        thread_sleep(1000000);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        thread_sleep(1000000);
    }
    return NULL;
}

int main(void)
{
    char *stack1 = malloc(THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE);
    char *stack2 = malloc(THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE);

    if (stack1 == NULL || stack2 == NULL) {
        printf("Failed to allocate stack memory\n");
        return 1;
    }

    thread1_pid = thread_create(stack1, THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE,
                                THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE,
                                THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    // Change priority of thread1 to higher priority
    sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN - 1);

    // Change priority of thread2 to lower priority
    sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
