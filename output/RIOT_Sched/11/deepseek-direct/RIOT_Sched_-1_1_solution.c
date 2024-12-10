
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static kernel_pid_t thread1_pid;
static kernel_pid_t thread2_pid;

static void *thread1(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(1);
    }
    return NULL;
}

static void *thread2(void *arg)
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

    // Create thread 1 with priority 3
    thread1_pid = thread_create(
        thread_stack(thread1),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST,
        thread1,
        NULL,
        "thread1"
    );

    // Create thread 2 with priority 4
    thread2_pid = thread_create(
        thread_stack(thread2),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST,
        thread2,
        NULL,
        "thread2"
    );

    // Change thread 1 priority to 5 after 5 seconds
    xtimer_sleep(5);
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);
    LOG_INFO("Changed Thread 1 priority to 5\n");

    // Change thread 2 priority to 6 after 10 seconds
    xtimer_sleep(5);
    thread_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 4);
    LOG_INFO("Changed Thread 2 priority to 6\n");

    return 0;
}
