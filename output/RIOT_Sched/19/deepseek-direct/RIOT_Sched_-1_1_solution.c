
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running at priority %d\n", thread_get_priority(pid1));
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running at priority %d\n", thread_get_priority(pid2));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Create thread 1 with default priority
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");

    // Create thread 2 with default priority
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    // Change thread 1 priority to higher
    thread_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);

    // Change thread 2 priority to lower
    thread_change_priority(pid2, THREAD_PRIORITY_MAIN);

    while (1) {
        LOG_INFO("Main thread running\n");
        xtimer_sleep(2);
    }

    return 0;
}
