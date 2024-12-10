#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 7

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                                              thread_func1, NULL, "thread1");
    kernel_pid_t thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0,
                                              thread_func2, NULL, "thread2");

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
        printf("Changing priority of threads\n");
        thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 1);
        thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN + 1);
        ztimer_sleep(ZTIMER_MSEC, 5000);
        printf("Reverting priority of threads\n");
        thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN);
        thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN);
    }
    return 0;
}