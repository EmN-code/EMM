
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

kernel_pid_t thread_pid1, thread_pid2;

void *thread_handler1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_handler2(void *arg)
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
    char stack1[THREAD_STACKSIZE];
    char stack2[THREAD_STACKSIZE];

    thread_pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY,
                                THREAD_CREATE_STACKTEST, thread_handler1, NULL, "thread1");

    thread_pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY,
                                THREAD_CREATE_STACKTEST, thread_handler2, NULL, "thread2");

    xtimer_sleep(5);

    sched_change_priority(thread_get(thread_pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread_pid2), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
