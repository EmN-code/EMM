
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MEDIUM)
#define THREAD_PRIORITY_DEFAULT (THREAD_PRIORITY_MAIN - 1)

char thread_stack1[THREAD_STACKSIZE_DEFAULT];
char thread_stack2[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t thread_pid1;
kernel_pid_t thread_pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
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
    thread_pid1 = thread_create(thread_stack1, sizeof(thread_stack1),
                                THREAD_PRIORITY_DEFAULT, THREAD_CREATE_STACKTEST,
                                thread_func1, NULL, "thread1");

    thread_pid2 = thread_create(thread_stack2, sizeof(thread_stack2),
                                THREAD_PRIORITY_DEFAULT, THREAD_CREATE_STACKTEST,
                                thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    // Change priority of thread 1
    sched_change_priority(thread_get(thread_pid1), THREAD_PRIORITY_MAIN - 2);

    // Change priority of thread 2
    sched_change_priority(thread_get(thread_pid2), THREAD_PRIORITY_MAIN - 3);

    return 0;
}
