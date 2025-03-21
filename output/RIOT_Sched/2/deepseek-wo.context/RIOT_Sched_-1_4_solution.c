
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (512)  // Define the stack size explicitly

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_WOUT_YIELD, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_WOUT_YIELD, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread1 to MAIN - 2\n");
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(5);

    printf("Changing priority of thread2 to MAIN - 1\n");
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
