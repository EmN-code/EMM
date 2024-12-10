
#include "thread.h"
#include "xtimer.h"
#include "msg.h"

#define THREAD1_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define THREAD2_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

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
    char thread1_stack[THREAD1_STACKSIZE];
    char thread2_stack[THREAD2_STACKSIZE];

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                               THREAD_PRIORITY_MAIN - 2,
                               THREAD_CREATE_STACKTEST,
                               thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing thread priorities\n");
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
