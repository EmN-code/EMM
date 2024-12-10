#include <thread.h>
#include <stdio.h>
#include <stdint.h>

#define STACK_SIZE  (THREAD_STACKSIZE_MAIN)
#define PRIORITY    (THREAD_PRIORITY_MAIN - 1)

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread_handler1(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_handler2(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1),
                                PRIORITY, THREAD_CREATE_STACKTEST,
                                thread_handler1, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2),
                                PRIORITY, THREAD_CREATE_STACKTEST,
                                thread_handler2, NULL, "thread2");

    // Simulating priority change after threads have been created
    sched_change_priority(thread_get(thread1_pid), PRIORITY + 1);
    sched_change_priority(thread_get(thread2_pid), PRIORITY + 2);

    return 0;
}