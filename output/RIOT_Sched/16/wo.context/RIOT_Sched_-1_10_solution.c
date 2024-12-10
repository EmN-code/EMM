#include "xtimer.h"
#include "thread.h"
#include <stdio.h>

#define THREAD_STACKSIZE_LARGE   (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

char stack1[THREAD_STACKSIZE_LARGE];
char stack2[THREAD_STACKSIZE_LARGE];

void *thread1_loop(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 1\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_loop(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 2\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;
    
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_loop, NULL, "thread1");
    
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_loop, NULL, "thread2");
    
    /* let the threads run for a while */
    xtimer_sleep(2);
    
    /* Change priorities at runtime */
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    return 0;
}