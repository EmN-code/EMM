
#include "thread.h"
#include "xtimer.h"
#include "msg.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "unistd.h"
#include "stdatomic.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void change_priorities(void)
{
    sched_change_priority(pid1, PRIORITY_LOW);
    sched_change_priority(pid2, PRIORITY_HIGH);
    printf("Priorities changed: Thread 1 to LOW, Thread 2 to HIGH\n");
}

int main(void)
{
    pid1 = thread_create(stack1, STACKSIZE, PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    xtimer_sleep(5);
    change_priorities();

    return 0;
}
