#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "sched.h"
#include "xtimer.h"

#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread %d is running\n", thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    thread_create(stack1, sizeof(stack1), PRIORITY, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread1");
    thread_create(stack2, sizeof(stack2), PRIORITY, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread2");

    while (1) {
        printf("Main thread is running\n");
        xtimer_sleep(2);
    }

    return 0;
}
