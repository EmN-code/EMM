#include "xtimer.h"
#include "stdio.h"
#include "sched.h"
#include "thread_arch.h"

#define THREAD_STACKSIZE_MAIN    (4 * 1024)
#define MY_THREAD_PRIORITY         (6) //custom priority, assuming NOT redefining existing

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_1(void *arg)
{
    (void) arg;
    printf("Thread 1 running with priority %d\n", thread_get_priority(thread_getpid()));
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_2(void *arg)
{
    (void) arg;
    printf("Thread 2 running with priority %d\n", thread_get_priority(thread_getpid()));
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid_one, pid_two;
    pid_one = thread_create(stack_one, sizeof(stack_one), MY_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, thread_1, NULL, "thread_1");
    pid_two = thread_create(stack_two, sizeof(stack_two), MY_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, thread_2, NULL, "thread_2");

    printf("Main: created threads with PIDs %d and %d\n", pid_one, pid_two);

    // Change priorities at runtime
    sched_change_priority(thread_get(pid_one), MY_THREAD_PRIORITY - 1);  // change priority to a higher level
    sched_change_priority(thread_get(pid_two), MY_THREAD_PRIORITY + 1);  // change priority to a lower level

    while (1) {
        xtimer_sleep(5);
    }
    return 0;
}