#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "thread_config.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread_one;
kernel_pid_t thread_two;

void *thread_one_func(void *arg) {
    while (1) {
        // Simulate work by sleeping
        ztimer_sleep(ZTIMER_SEC, 2);
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    while (1) {
        // Simulate work by sleeping
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

int main(void) {
    thread_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN + 1,
                               0, thread_one_func, NULL, "Thread One");
    
    thread_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 2,
                               0, thread_two_func, NULL, "Thread Two");

    // Change the priority of the threads at runtime
    sched_change_priority(thread_get(thread_one), THREAD_PRIORITY_MAIN + 3);
    sched_change_priority(thread_get(thread_two), THREAD_PRIORITY_MAIN + 4);

    return 0;
}