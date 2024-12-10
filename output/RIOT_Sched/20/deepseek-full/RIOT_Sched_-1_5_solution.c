
#include "thread.h"
#include "xtimer.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "kernel_defines.h"

#define STACKSIZE 512
#define PRIORITY 5

kernel_pid_t thread1_pid, thread2_pid;
char stack1[STACKSIZE], stack2[STACKSIZE];

void *thread1_func(void *arg) {
    (void)arg;
    printf("Thread 1 started\n");
    xtimer_sleep(5);
    printf("Thread 1 finished\n");
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    printf("Thread 2 started\n");
    xtimer_sleep(5);
    printf("Thread 2 finished\n");
    return NULL;
}

void change_thread_priorities(void) {
    thread_set_priority(thread1_pid, PRIORITY + 1);
    thread_set_priority(thread2_pid, PRIORITY + 2);
    printf("Thread priorities changed\n");
}

int main(void) {
    printf("Main thread started\n");

    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(10);
    change_thread_priorities();

    printf("Main thread finished\n");
    return 0;
}
