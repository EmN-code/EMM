
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdatomic.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t pid1, pid2;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", sched_get_priority(sched_active_pid));
    while (1) {
        printf("Thread 1 running\n");
        sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", sched_get_priority(sched_active_pid));
    while (1) {
        printf("Thread 2 running\n");
        sleep(1);
    }
    return NULL;
}

void change_thread_priority(kernel_pid_t pid, uint8_t new_priority)
{
    sched_set_priority(pid, new_priority);
    printf("Priority of thread %d changed to %d\n", pid, new_priority);
}

int main(void)
{
    puts("RIOT multi-threading example");

    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    sleep(5);

    change_thread_priority(pid1, PRIORITY_LOW);
    change_thread_priority(pid2, PRIORITY_HIGH);

    return 0;
}
