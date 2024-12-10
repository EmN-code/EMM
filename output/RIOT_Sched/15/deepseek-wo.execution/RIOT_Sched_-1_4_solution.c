
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sched.h>
#include <thread.h>

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_LOW 10

static char stack1[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];

void *thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 running at priority %d\n", sched_get_priority(thread_get_active()));
    while (1) {
        sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 running at priority %d\n", sched_get_priority(thread_get_active()));
    while (1) {
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    printf("Main thread running at priority %d\n", sched_get_priority(thread_get_active()));

    sleep(5);

    printf("Changing priority of Thread 1 to LOW\n");
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_LOW);

    sleep(5);

    printf("Changing priority of Thread 2 to MAIN\n");
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN);

    while (1) {
        sleep(1);
    }

    return 0;
}
