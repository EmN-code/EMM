
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

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

    // Create and start thread 1
    thread1_pid = thread_create(
        thread_stack_init(THREAD_STACKSIZE_MAIN),
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    // Create and start thread 2
    thread2_pid = thread_create(
        thread_stack_init(THREAD_STACKSIZE_MAIN),
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    // Change priority of thread 1 to a higher priority
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 1);

    // Change priority of thread 2 to a lower priority
    thread_change_priority(thread2_pid, THREAD_PRIORITY_MAIN + 1);

    // Main thread will also run and print its priority
    while (1) {
        LOG_INFO("Main thread running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }

    return 0;
}
