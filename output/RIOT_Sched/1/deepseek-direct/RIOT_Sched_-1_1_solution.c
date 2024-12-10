
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static kernel_pid_t thread_pid1;
static kernel_pid_t thread_pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_pid1 = thread_create(stack1, sizeof(stack1),
                                THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST,
                                thread_func1, NULL, "thread1");

    thread_pid2 = thread_create(stack2, sizeof(stack2),
                                THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST,
                                thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    LOG_INFO("Changing priority of Thread 1 to higher\n");
    thread_change_priority(thread_pid1, THREAD_PRIORITY_MAIN - 3, false);

    xtimer_sleep(5);

    LOG_INFO("Changing priority of Thread 2 to higher\n");
    thread_change_priority(thread_pid2, THREAD_PRIORITY_MAIN - 4, false);

    return 0;
}
