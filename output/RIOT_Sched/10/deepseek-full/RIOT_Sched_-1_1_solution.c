
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN - 1)

kernel_pid_t thread_pid_1;
kernel_pid_t thread_pid_2;

void *thread_func_1(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func_2(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    // Initialize logging
    log_init();

    // Create and start the first thread
    thread_pid_1 = thread_create(
        thread_stack,
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        thread_func_1,
        NULL,
        "thread_1"
    );

    // Create and start the second thread
    thread_pid_2 = thread_create(
        thread_stack,
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        thread_func_2,
        NULL,
        "thread_2"
    );

    // Change the priority of the second thread at runtime
    thread_t *thread_2 = thread_get(thread_pid_2);
    sched_change_priority(thread_2, THREAD_PRIORITY_MAIN);

    // Main thread will also run and print a message every 2 seconds
    while (1) {
        LOG_INFO("Main thread running\n");
        xtimer_sleep(2);
    }

    return 0;
}
