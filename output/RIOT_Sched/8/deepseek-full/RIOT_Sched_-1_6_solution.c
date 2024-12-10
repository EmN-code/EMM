
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdatomic.h>
#include <sched.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void change_priorities(void) {
    sched_change_priority(thread1_pid, PRIORITY_LOW);
    sched_change_priority(thread2_pid, PRIORITY_HIGH);
    printf("Priorities changed: Thread 1 to LOW, Thread 2 to HIGH\n");
}

int main(void) {
    char stack1[STACKSIZE], stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, STACKSIZE, PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, STACKSIZE, PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    printf("Threads created\n");

    xtimer_sleep(5);
    change_priorities();

    return 0;
}
