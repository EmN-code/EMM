
#include "thread.h"
#include "sched.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

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
    kernel_pid_t pid1, pid2;
    thread_t *thread1, *thread2;

    pid1 = thread_create(thread1_stack, sizeof(thread1_stack),
                         THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                         thread1_func, NULL, "thread1");

    pid2 = thread_create(thread2_stack, sizeof(thread2_stack),
                         THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                         thread2_func, NULL, "thread2");

    // Get the thread structures
    thread1 = thread_get(pid1);
    thread2 = thread_get(pid2);

    // Change priority at runtime
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 4);

    while (1) {
        printf("Main thread running\n");
        xtimer_sleep(2);
    }

    return 0;
}
