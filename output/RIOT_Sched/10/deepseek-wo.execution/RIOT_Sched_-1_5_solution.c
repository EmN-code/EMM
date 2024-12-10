
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <thread.h>
#include <sched.h>

#define STACKSIZE 512

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func2, NULL, "thread2");

    sleep(5);

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    while (1) {
        printf("Main thread running\n");
        sleep(1);
    }

    return 0;
}
