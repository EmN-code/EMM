
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <kernel_defines.h>

#define STACKSIZE 1024
#define THREAD_PRIORITY_HIGH 7
#define THREAD_PRIORITY_LOW 15

char stack0[STACKSIZE];
char stack1[STACKSIZE];

void *thread_func_0(void *arg) {
    (void)arg;
    printf("Thread 0 started with priority %d\n", sched_active_thread_priority());
    while (1) {
        printf("Thread 0 running\n");
        sleep(1);
    }
    return NULL;
}

void *thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 started with priority %d\n", sched_active_thread_priority());
    while (1) {
        printf("Thread 1 running\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid0, pid1;

    pid0 = thread_create(stack0, sizeof(stack0), THREAD_PRIORITY_HIGH, 0, thread_func_0, NULL, "thread0");
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_LOW, 0, thread_func_1, NULL, "thread1");

    printf("Main thread started\n");

    sleep(5);

    printf("Changing priority of Thread 0 to LOW\n");
    sched_change_priority(thread_get(pid0), THREAD_PRIORITY_LOW);

    sleep(5);

    printf("Changing priority of Thread 1 to HIGH\n");
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_HIGH);

    while (1) {
        sleep(1);
    }

    return 0;
}
