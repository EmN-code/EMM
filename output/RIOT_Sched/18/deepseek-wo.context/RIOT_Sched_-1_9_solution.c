
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2(void *arg)
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
    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    // Change the priority of the first thread
    thread_t *thread1_tcb = thread_get(pid1);
    sched_change_priority(thread1_tcb, THREAD_PRIORITY_MAIN - 2);

    // Change the priority of the second thread
    thread_t *thread2_tcb = thread_get(pid2);
    sched_change_priority(thread2_tcb, THREAD_PRIORITY_MAIN - 2);

    // Main thread continues to run
    while (1) {
        printf("Main thread running\n");
        xtimer_sleep(2);
    }

    return 0;
}
