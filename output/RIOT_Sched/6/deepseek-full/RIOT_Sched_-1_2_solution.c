
#include "sched.h"
#include "xtimer.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid, thread2_pid;
char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        sched_change_priority(thread2_pid, 1);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), 2, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), 3, 0, thread2_func, NULL, "thread2");

    while (1) {
        printf("Main thread running\n");
        xtimer_sleep(5);
    }

    return 0;
}
