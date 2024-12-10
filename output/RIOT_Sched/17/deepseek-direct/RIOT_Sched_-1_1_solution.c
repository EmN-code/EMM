
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack1[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];

static kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
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
    LOG_INFO("Starting two threads with priority change at runtime\n");

    // Create the first thread with default priority
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    // Create the second thread with default priority
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Change the priority of the first thread to higher
    thread_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);

    // Change the priority of the second thread to lower
    thread_change_priority(pid2, THREAD_PRIORITY_MAIN);

    // Main thread will also run and print a message every 5 seconds
    while (1) {
        LOG_INFO("Main thread running\n");
        xtimer_sleep(5);
    }

    return 0;
}
