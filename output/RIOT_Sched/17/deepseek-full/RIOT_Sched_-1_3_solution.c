
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE (1024)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char stack_thread1[STACKSIZE];
char stack_thread2[STACKSIZE];

kernel_pid_t pid_thread1;
kernel_pid_t pid_thread2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at high priority\n");
        xtimer_sleep(5);
        thread_change_priority(pid_thread1, PRIORITY_LOW);
        printf("Thread 1 priority changed to low\n");
        xtimer_sleep(5);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at low priority\n");
        xtimer_sleep(10);
        thread_change_priority(pid_thread2, PRIORITY_HIGH);
        printf("Thread 2 priority changed to high\n");
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    pid_thread1 = thread_create(stack_thread1, sizeof(stack_thread1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid_thread2 = thread_create(stack_thread2, sizeof(stack_thread2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    return 0;
}
