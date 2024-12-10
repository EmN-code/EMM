#include <ztimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sched.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <thread_config.h>
#include <thread.h>
#include <mbox.h>

#define STACK_SIZE_THREAD    (KERNEL_CONF_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_1    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2    (THREAD_PRIORITY_MAIN - 2)

char stack1[STACK_SIZE_THREAD];
char stack2[STACK_SIZE_THREAD];

static kernel_pid_t thread1_pid;
static kernel_pid_t thread2_pid;

void *thread1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
        sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_2);
    }
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
        sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_1);
    }
    return NULL;
}

int main(void) {
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1, 0, thread1, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2, 0, thread2, NULL, "thread2");

    while (1) {
        thread_yield();
    }
    return 0;
}