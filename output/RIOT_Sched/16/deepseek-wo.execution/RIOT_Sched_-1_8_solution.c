
#include <stdio.h>
#include <stdlib.h>
#include <kernel_defines.h>
#include <thread.h>
#include <sched.h>

#define STACK_SIZE 512
#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread_func1(void *arg) {
    printf("Thread 1 running with priority %d\n", sched_active_thread_priority());
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 running with priority %d\n", sched_active_thread_priority());
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    if (pid1 <= 0) {
        printf("Error creating thread 1\n");
        return 1;
    }

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_LOW, 0, thread_func2, NULL, "thread2");
    if (pid2 <= 0) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change the priority of the first thread to low
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_LOW);

    // Change the priority of the second thread to high
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_HIGH);

    // Let the threads run for a while
    sleep(5);

    return 0;
}
