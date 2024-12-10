
#include "thread.h"
#include "xtimer.h"
#include "msg.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

#define THREAD_PRIORITY_1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2 (THREAD_PRIORITY_MAIN - 2)

char thread_stack_1[THREAD_STACKSIZE_DEFAULT];
char thread_stack_2[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid_1, pid_2;

void *thread_func_1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_getpid_of(thread_get_active())));
        xtimer_sleep(5);
    }
    return NULL;
}

void *thread_func_2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_getpid_of(thread_get_active())));
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    pid_1 = thread_create(thread_stack_1, sizeof(thread_stack_1), THREAD_PRIORITY_1,
                          THREAD_CREATE_STACKTEST, thread_func_1, NULL, "thread_1");

    pid_2 = thread_create(thread_stack_2, sizeof(thread_stack_2), THREAD_PRIORITY_2,
                          THREAD_CREATE_STACKTEST, thread_func_2, NULL, "thread_2");

    xtimer_sleep(10);

    printf("Changing priority of Thread 1 to %d\n", THREAD_PRIORITY_2);
    sched_change_priority(thread_get_from_pid(pid_1), THREAD_PRIORITY_2);

    xtimer_sleep(10);

    printf("Changing priority of Thread 2 to %d\n", THREAD_PRIORITY_1);
    sched_change_priority(thread_get_from_pid(pid_2), THREAD_PRIORITY_1);

    return 0;
}
