
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

char thread_stack1[THREAD_STACKSIZE_MAIN];
char thread_stack2[THREAD_STACKSIZE_MAIN];

kernel_pid_t pid1, pid2;

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
    pid1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    pid2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    sched_change_priority(thread_get_thread_pid(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get_thread_pid(pid2), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
