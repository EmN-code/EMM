
#include "thread.h"
#include "xtimer.h"
#include "log.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        log_info("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        log_info("Thread 2 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    log_init();

    char stack1[THREAD_STACKSIZE];
    char stack2[THREAD_STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    log_info("Changing thread priorities\n");
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}