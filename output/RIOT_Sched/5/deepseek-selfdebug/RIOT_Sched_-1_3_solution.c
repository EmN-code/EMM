
#include "thread.h"
#include "xtimer.h"
#include "msg.h"

#define THREAD1_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD2_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

#define THREAD1_PRIO (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIO (THREAD_PRIORITY_MAIN - 2)

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    char thread1_stack[THREAD1_STACKSIZE];
    char thread2_stack[THREAD2_STACKSIZE];

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD1_PRIO,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD2_PRIO,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 1 to MAIN\n");
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN);

    xtimer_sleep(5);

    printf("Changing priority of Thread 2 to MAIN\n");
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN);

    return 0;
}
