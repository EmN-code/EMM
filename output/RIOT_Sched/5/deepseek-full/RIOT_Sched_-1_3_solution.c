
#include <stdio.h>
#include <ztimer.h>
#include <thread.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <xtimer.h>
#include <sched.h>
#include <msg.h>

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void)
{
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(2); // Wait for threads to start

    // Change priority of thread1
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread1\n");

    xtimer_sleep(2); // Wait for priority change to take effect

    // Change priority of thread2
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread2\n");

    return 0;
}
