
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <sched.h>

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread1_func(void *arg) {
    (void)arg;
    printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create thread 1 with high priority
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    if (pid1 < 0) {
        printf("Error creating thread 1\n");
        return 1;
    }

    // Create thread 2 with low priority
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread2_func, NULL, "thread2");
    if (pid2 < 0) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change the priority of thread 1 to low
    sched_change_priority(thread_get(pid1), PRIORITY_LOW);
    printf("Changed priority of thread 1 to %d\n", PRIORITY_LOW);

    // Change the priority of thread 2 to high
    sched_change_priority(thread_get(pid2), PRIORITY_HIGH);
    printf("Changed priority of thread 2 to %d\n", PRIORITY_HIGH);

    return 0;
}
