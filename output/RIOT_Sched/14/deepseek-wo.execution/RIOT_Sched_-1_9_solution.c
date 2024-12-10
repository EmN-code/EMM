
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10

char thread1_stack[THREAD_STACKSIZE];
char thread2_stack[THREAD_STACKSIZE];

void *thread1_func(void *arg) {
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_get_priority(thread_getpid()));
        sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_get_priority(thread_getpid()));
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(thread1_stack, THREAD_STACKSIZE, THREAD_PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(thread2_stack, THREAD_STACKSIZE, THREAD_PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    sleep(5);

    printf("Changing priority of Thread 1 to LOW\n");
    sched_change_priority(pid1, THREAD_PRIORITY_LOW);

    sleep(5);

    printf("Changing priority of Thread 2 to HIGH\n");
    sched_change_priority(pid2, THREAD_PRIORITY_HIGH);

    while (1) {
        sleep(1);
    }

    return 0;
}
