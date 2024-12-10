
#include "thread.h"
#include "xtimer.h"
#include "msg.h"

#define THREAD1_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)
#define THREAD2_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(5);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    char thread1_stack[THREAD1_STACKSIZE];
    char thread2_stack[THREAD2_STACKSIZE];

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(10);

    printf("Changing thread1 priority to MAIN - 2\n");
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(10);

    printf("Changing thread2 priority to MAIN - 1\n");
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
