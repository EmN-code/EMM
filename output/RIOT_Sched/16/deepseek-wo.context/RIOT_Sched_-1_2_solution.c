
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <irq.h>
#include <sched.h>

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE)
#define THREAD_PRIORITY_DEFAULT (THREAD_PRIORITY_MAIN - 1)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_DEFAULT,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_DEFAULT,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priorities...\n");

    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN);
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 2);

    return 0;
}
