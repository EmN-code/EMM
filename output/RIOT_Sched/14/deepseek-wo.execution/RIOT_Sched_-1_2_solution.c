
#include "ztimer.h"
#include "xtimer.h"
#include "event/thread.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static char stack_thread1[THREAD_STACKSIZE_MAIN];
static char stack_thread2[THREAD_STACKSIZE_MAIN];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t pid1 = thread_create(stack_thread1, sizeof(stack_thread1),
                                      THREAD_PRIORITY_MAIN - 1, 0,
                                      thread1_func, NULL, "thread1");

    kernel_pid_t pid2 = thread_create(stack_thread2, sizeof(stack_thread2),
                                      THREAD_PRIORITY_MAIN - 2, 0,
                                      thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread1 to higher\n");
    thread_change_priority(pid1, THREAD_PRIORITY_MAIN - 3, false);

    xtimer_sleep(5);

    printf("Changing priority of thread2 to higher\n");
    thread_change_priority(pid2, THREAD_PRIORITY_MAIN - 3, false);

    return 0;
}
