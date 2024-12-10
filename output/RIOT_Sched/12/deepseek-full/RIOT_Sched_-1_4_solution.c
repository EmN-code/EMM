
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "sched.h"
#include "xtimer.h"
#include "thread.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

void *thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread_2");

    // Change priority of thread 1
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN + 1);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN + 2);

    printf("Main thread exiting\n");
    return 0;
}
