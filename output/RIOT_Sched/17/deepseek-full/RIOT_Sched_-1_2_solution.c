
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

static kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

void change_priority(kernel_pid_t pid, uint8_t new_priority)
{
    sched_set_status(sched_threads[pid], STATUS_RUNNING);
    sched_threads[pid]->priority = new_priority;
}

int main(void)
{
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread 1 to LOW\n");
    change_priority(pid1, PRIORITY_LOW);

    xtimer_sleep(5);

    printf("Changing priority of thread 2 to HIGH\n");
    change_priority(pid2, PRIORITY_HIGH);

    return 0;
}
