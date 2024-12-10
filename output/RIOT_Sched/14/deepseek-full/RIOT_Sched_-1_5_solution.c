
#include "thread.h"
#include "sched.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    if (pid1 <= 0 || pid2 <= 0) {
        printf("Failed to create threads\n");
        return 1;
    }

    xtimer_sleep(5);

    // Change priority of thread 1 to low
    sched_change_priority(thread_get(pid1), PRIORITY_LOW);
    printf("Changed priority of thread 1 to low\n");

    xtimer_sleep(5);

    // Change priority of thread 2 to high
    sched_change_priority(thread_get(pid2), PRIORITY_HIGH);
    printf("Changed priority of thread 2 to high\n");

    return 0;
}
