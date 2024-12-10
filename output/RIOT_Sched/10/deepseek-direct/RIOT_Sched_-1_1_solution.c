
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static kernel_pid_t thread1_pid;
static kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
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

    thread1_pid = thread_create(stack1, sizeof(stack1),
                                THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2),
                                THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    LOG_INFO("Changing priority of Thread 1 to higher\n");
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    xtimer_sleep(5);

    LOG_INFO("Changing priority of Thread 2 to higher\n");
    thread_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 4);

    return 0;
}
