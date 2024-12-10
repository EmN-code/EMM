
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

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
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 1\n");
        return 1;
    }

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change the priority of the first thread at runtime
    thread_t *thread1_ptr = thread_get(pid1);
    if (thread1_ptr != NULL) {
        sched_change_priority(thread1_ptr, THREAD_PRIORITY_MAIN - 2);
    }

    // Change the priority of the second thread at runtime
    thread_t *thread2_ptr = thread_get(pid2);
    if (thread2_ptr != NULL) {
        sched_change_priority(thread2_ptr, THREAD_PRIORITY_MAIN - 3);
    }

    return 0;
}
