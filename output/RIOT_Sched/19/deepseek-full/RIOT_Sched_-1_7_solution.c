
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;
    uint8_t priority1, priority2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    printf("Threads created with PIDs: %d and %d\n", pid1, pid2);

    xtimer_sleep(5);

    priority1 = thread_get_priority(thread_get_from_pid(pid1));
    priority2 = thread_get_priority(thread_get_from_pid(pid2));

    printf("Initial priorities: Thread 1: %d, Thread 2: %d\n", priority1, priority2);

    thread_set_priority(pid1, PRIORITY_LOW);
    thread_set_priority(pid2, PRIORITY_HIGH);

    priority1 = thread_get_priority(thread_get_from_pid(pid1));
    priority2 = thread_get_priority(thread_get_from_pid(pid2));

    printf("Updated priorities: Thread 1: %d, Thread 2: %d\n", priority1, priority2);

    return 0;
}
