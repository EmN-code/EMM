
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <stdlib.h>

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    printf("Thread 1 started\n");
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    printf("Thread 2 started\n");
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void change_priorities(void) {
    printf("Changing priorities...\n");
    thread_set_priority(thread1_pid, PRIORITY_LOW);
    thread_set_priority(thread2_pid, PRIORITY_HIGH);
    printf("Priorities changed\n");
}

int main(void) {
    char stack1[STACKSIZE], stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    if (thread1_pid <= 0 || thread2_pid <= 0) {
        printf("Error creating threads\n");
        return 1;
    }

    printf("Threads created successfully\n");

    xtimer_sleep(5);
    change_priorities();

    return 0;
}
