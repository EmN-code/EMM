
#include "evtimer_msg.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "time_units.h"

#define THREAD_STACKSIZE_DEFAULT (512)  // Define the stack size here to avoid redefinition error

kernel_pid_t thread1_pid, thread2_pid;

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
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change priority of thread1 to a higher priority
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread2 to a lower priority
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
