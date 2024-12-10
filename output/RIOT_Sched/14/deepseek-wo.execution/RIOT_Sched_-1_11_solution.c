
#include <stdio.h>
#include <thread.h>
#include <sched.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack0[STACKSIZE];
char stack1[STACKSIZE];

void *thread_func0(void *arg) {
    (void)arg;
    printf("Thread 0 started with priority %d\n", thread_get_priority(thread_get_active()));
    while (1) {
        thread_yield();
    }
    return NULL;
}

void *thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 started with priority %d\n", thread_get_priority(thread_get_active()));
    while (1) {
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid0, pid1;

    pid0 = thread_create(stack0, sizeof(stack0), PRIORITY_HIGH, 0, thread_func0, NULL, "thread0");
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_LOW, 0, thread_func1, NULL, "thread1");

    printf("Main thread started\n");

    // Change priority of thread 0 to low
    sched_change_priority(thread_get(pid0), PRIORITY_LOW);
    printf("Changed priority of thread 0 to %d\n", PRIORITY_LOW);

    // Change priority of thread 1 to high
    sched_change_priority(thread_get(pid1), PRIORITY_HIGH);
    printf("Changed priority of thread 1 to %d\n", PRIORITY_HIGH);

    while (1) {
        thread_yield();
    }

    return 0;
}
