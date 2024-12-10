
#include "ztimer.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "time_units.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static char stack_thread1[THREAD_STACKSIZE_MAIN];
static char stack_thread2[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread1_pid = thread_create(stack_thread1, sizeof(stack_thread1),
                                THREAD_PRIORITY_MAIN - 1, 0,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack_thread2, sizeof(stack_thread2),
                                THREAD_PRIORITY_MAIN - 2, 0,
                                thread2_func, NULL, "thread2");

    // Change priority of thread1 to higher than thread2
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    return 0;
}
