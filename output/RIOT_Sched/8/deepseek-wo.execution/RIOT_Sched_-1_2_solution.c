
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

#define THREAD_STACKSIZE    (512)
#define THREAD1_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIO        (THREAD_PRIORITY_MAIN - 2)

static char thread1_stack[THREAD_STACKSIZE];
static char thread2_stack[THREAD_STACKSIZE];

static void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

static void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                             THREAD1_PRIO, THREAD_CREATE_STACKTEST,
                                             thread1_func, NULL, "thread1");

    kernel_pid_t thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                             THREAD2_PRIO, THREAD_CREATE_STACKTEST,
                                             thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 1 to %d\n", THREAD2_PRIO);
    sched_change_priority(thread_get(thread1_pid), THREAD2_PRIO);

    xtimer_sleep(5);

    printf("Changing priority of Thread 2 to %d\n", THREAD1_PRIO);
    sched_change_priority(thread_get(thread2_pid), THREAD1_PRIO);

    return 0;
}
