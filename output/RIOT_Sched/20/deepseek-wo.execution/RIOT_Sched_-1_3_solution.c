
#include "sched.h"
#include "thread.h"
#include "msg.h"
#include "unistd.h"
#include "stdbool.h"
#include "stdlib.h"

#define THREAD_STACKSIZE (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN - 1)

char thread_stack1[THREAD_STACKSIZE];
char thread_stack2[THREAD_STACKSIZE];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(thread_stack1, sizeof(thread_stack1),
                         THREAD_PRIORITY_MAIN, 0, thread_func1, NULL, "thread1");

    pid2 = thread_create(thread_stack2, sizeof(thread_stack2),
                         THREAD_PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    // Change priority of thread 1 to low
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_LOW);

    // Change priority of thread 2 to main
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN);

    return 0;
}
