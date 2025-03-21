
#include "thread.h"
#include "sched.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

kernel_pid_t thread1_pid, thread2_pid;

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
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change priority of thread1 to a higher priority
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);

    // Change priority of thread2 to a lower priority
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);

    return 0;
}
