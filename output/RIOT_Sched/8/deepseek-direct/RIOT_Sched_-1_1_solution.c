
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE 512

static char stack1[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];

static kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    // Initialize logging
    LOG_INFO("Starting RIOT application on ESP32\n");

    // Create the first thread with priority 3
    pid1 = thread_create(stack1, sizeof(stack1), 3, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    // Create the second thread with priority 4
    pid2 = thread_create(stack2, sizeof(stack2), 4, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Change the priority of the first thread to 5 after 5 seconds
    xtimer_sleep(5);
    thread_change_priority(pid1, 5);
    LOG_INFO("Changed priority of Thread 1 to 5\n");

    // Change the priority of the second thread to 2 after 10 seconds
    xtimer_sleep(5);
    thread_change_priority(pid2, 2);
    LOG_INFO("Changed priority of Thread 2 to 2\n");

    return 0;
}
