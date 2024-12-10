#include "thread_arch.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include "architecture.h"

#include <stdio.h>
#include <stdlib.h>

#define THREAD1_STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD2_STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

char stack1[THREAD1_STACKSIZE];
char stack2[THREAD2_STACKSIZE];

void *thread1_func(void *arg) {
    while (1) {
        printf("Thread 1 running with priority %d\n", thread_get_active()->priority);
        thread_yield();
        sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN - 1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    while (1) {
        printf("Thread 2 running with priority %d\n", thread_get_active()->priority);
        thread_yield();
        sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN + 1);
    }
    return NULL;
}

int main(void) {
    printf("Starting RIOT threads on ESP32\n");

    kernel_pid_t t1 = thread_create(stack1, THREAD1_STACKSIZE, THREAD_PRIORITY_MAIN, CREATE_STACKTEST, thread1_func, NULL, "thread1");
    kernel_pid_t t2 = thread_create(stack2, THREAD2_STACKSIZE, THREAD_PRIORITY_MAIN, CREATE_STACKTEST, thread2_func, NULL, "thread2");

    while (1) {
        thread_yield();
    }

    return 0;
}