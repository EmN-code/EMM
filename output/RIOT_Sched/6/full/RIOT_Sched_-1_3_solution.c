#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "stddef.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "architecture.h"
#include "stdio.h"
#include "sched.h"

void *thread1_handler(void *arg) {
    while (1) {
        // Simulate work
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    while (1) {
        // Simulate work
        ztimer_sleep(ZTIMER_USEC, 2000000);
    }
    return NULL;
}

int main(void) {
    // Create thread 1
    static char stack1[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread1_handler, NULL, "thread1");

    // Create thread 2
    static char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread2_handler, NULL, "thread2");

    ztimer_sleep(ZTIMER_USEC, 5000000);

    // Change priority of thread 1
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 4);

    while (1) {}
    return 0;
}