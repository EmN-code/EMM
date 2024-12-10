
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;
    uint8_t priority1, priority2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Get the current priority of the threads
    priority1 = thread_get_priority(pid1);
    priority2 = thread_get_priority(pid2);

    printf("Initial priority of thread1: %u\n", priority1);
    printf("Initial priority of thread2: %u\n", priority2);

    // Change the priority of the threads at runtime
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 3);

    // Get the new priority of the threads
    priority1 = thread_get_priority(pid1);
    priority2 = thread_get_priority(pid2);

    printf("New priority of thread1: %u\n", priority1);
    printf("New priority of thread2: %u\n", priority2);

    return 0;
}
