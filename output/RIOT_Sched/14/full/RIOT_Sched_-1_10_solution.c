#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

void *thread1_entry(void *arg) {
    while (1) {
        printf("Thread 1 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread2_entry(void *arg) {
    while (1) {
        printf("Thread 2 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];
    thread_t thread1, thread2;

    if (thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1_entry, NULL, "thread1") <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    if (thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread2_entry, NULL, "thread2") <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    ztimer_sleep(ZTIMER_MSEC, 5000);

    printf("Changing thread priorities\n");
    sched_set_priority(thread_getpid_of(&thread1), THREAD_PRIORITY_MAIN - 3);
    sched_set_priority(thread_getpid_of(&thread2), THREAD_PRIORITY_MAIN - 4);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}