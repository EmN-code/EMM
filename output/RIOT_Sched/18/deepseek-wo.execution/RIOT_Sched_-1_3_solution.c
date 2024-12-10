
#include "thread.h"
#include "ztimer.h"
#include "msg.h"

#define STACKSIZE 512

kernel_pid_t thread1_pid, thread2_pid;
char stack1[STACKSIZE], stack2[STACKSIZE];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    ztimer_sleep(ZTIMER_MSEC, 5000);

    // Change priority of thread1 to higher than thread2
    thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    return 0;
}
