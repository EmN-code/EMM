
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
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
#include <kernel_defines.h>

#define THREAD_STACKSIZE_DEFAULT 512

void thread_func_1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        sleep(1);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        sleep(1);
    }
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread_2");

    sleep(5);

    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN + 1);

    while (1) {
        printf("Main thread running\n");
        sleep(1);
    }

    return 0;
}