
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY 5

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    char thread1_stack[THREAD_STACKSIZE];
    char thread2_stack[THREAD_STACKSIZE];

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY, 0, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY, 0, thread2_func, NULL, "thread2");

    // Change priority of thread1 at runtime
    thread_change_priority(thread1_pid, THREAD_PRIORITY + 1);

    // Change priority of thread2 at runtime
    thread_change_priority(thread2_pid, THREAD_PRIORITY - 1);

    return 0;
}
