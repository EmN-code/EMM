
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <msg.h>
#include <thread.h>

#define STACKSIZE 1024

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    sleep(5);

    printf("Changing priority of thread1 to higher\n");
    thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    sleep(5);

    printf("Changing priority of thread2 to higher\n");
    thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN - 4);

    return 0;
}
