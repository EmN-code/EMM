
#include "thread.h"
#include "xtimer.h"
#include "msg.h"
#include "stdio.h"
#include "stdlib.h"

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t thread1_pid, thread2_pid;
char stack1[STACKSIZE], stack2[STACKSIZE];

void *thread1_func(void *arg) {
    (void)arg;
    printf("Thread 1 started with PID %d\n", thread_getpid());
    xtimer_sleep(5);
    printf("Thread 1 changing priority\n");
    thread_set_priority(thread1_pid, PRIORITY_LOW);
    printf("Thread 1 priority changed to LOW\n");
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    printf("Thread 2 started with PID %d\n", thread_getpid());
    xtimer_sleep(5);
    printf("Thread 2 changing priority\n");
    thread_set_priority(thread2_pid, PRIORITY_HIGH);
    printf("Thread 2 priority changed to HIGH\n");
    return NULL;
}

int main(void) {
    printf("Starting main thread\n");

    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    printf("Threads created\n");

    return 0;
}
