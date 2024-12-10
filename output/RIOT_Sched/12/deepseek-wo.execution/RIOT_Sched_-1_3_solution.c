
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"
#include "ztimer.h"
#include "mbox.h"
#include "unistd.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
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
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1, 0,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2, 0,
                                thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread1 to higher\n");
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    xtimer_sleep(5);

    printf("Changing priority of thread2 to higher\n");
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 4);

    return 0;
}
