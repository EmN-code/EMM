
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static kernel_pid_t pid1, pid2;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
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
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Create the first thread with priority 3
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    // Create the second thread with priority 4
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change the priority of thread1 to 5 after 5 seconds
    xtimer_sleep(5);
    thread_change_priority(pid1, THREAD_PRIORITY_MAIN - 3);
    LOG_INFO("Changed priority of thread1 to %d\n", THREAD_PRIORITY_MAIN - 3);

    // Change the priority of thread2 to 6 after another 5 seconds
    xtimer_sleep(5);
    thread_change_priority(pid2, THREAD_PRIORITY_MAIN - 4);
    LOG_INFO("Changed priority of thread2 to %d\n", THREAD_PRIORITY_MAIN - 4);

    return 0;
}
