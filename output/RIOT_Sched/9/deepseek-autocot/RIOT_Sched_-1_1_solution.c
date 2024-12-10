
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN - 1)

kernel_pid_t pid_thread1, pid_thread2;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    // Initialize logging
    log_init();

    // Create thread 1
    pid_thread1 = thread_create(
        thread_stack_t(thread1),
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    // Create thread 2
    pid_thread2 = thread_create(
        thread_stack_t(thread2),
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    // Change priority of thread 2 at runtime
    xtimer_sleep(5);
    sched_change_priority(pid_thread2, THREAD_PRIORITY_MAIN);
    LOG_INFO("Changed priority of thread 2 to %d\n", THREAD_PRIORITY_MAIN);

    return 0;
}
