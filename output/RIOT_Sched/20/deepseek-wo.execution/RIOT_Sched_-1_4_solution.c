
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sched.h>
#include <stdlib.h>
#include <event/thread.h>
#include <ztimer.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <mbox.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <time_units.h>

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

void thread_func_1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

int main(void) {
    char stack_1[STACKSIZE];
    char stack_2[STACKSIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, STACKSIZE, PRIORITY_HIGH, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, STACKSIZE, PRIORITY_LOW, 0, thread_func_2, NULL, "thread_2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 1 to LOW\n");
    sched_change_priority(thread_get(pid_1), PRIORITY_LOW);

    xtimer_sleep(5);

    printf("Changing priority of Thread 2 to HIGH\n");
    sched_change_priority(thread_get(pid_2), PRIORITY_HIGH);

    return 0;
}
