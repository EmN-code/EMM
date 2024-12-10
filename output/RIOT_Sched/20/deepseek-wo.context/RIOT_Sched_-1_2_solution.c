
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

char stack_thread1[THREAD_STACKSIZE];
char stack_thread2[THREAD_STACKSIZE];

kernel_pid_t pid_thread1;
kernel_pid_t pid_thread2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    pid_thread1 = thread_create(stack_thread1, sizeof(stack_thread1), PRIORITY_HIGH,
                                THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    pid_thread2 = thread_create(stack_thread2, sizeof(stack_thread2), PRIORITY_LOW,
                                THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 1 to LOW\n");
    sched_change_priority(thread_get(pid_thread1), PRIORITY_LOW);

    xtimer_sleep(5);

    printf("Changing priority of Thread 2 to HIGH\n");
    sched_change_priority(thread_get(pid_thread2), PRIORITY_HIGH);

    return 0;
}
