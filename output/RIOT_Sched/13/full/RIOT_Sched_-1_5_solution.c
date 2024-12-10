#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "unistd.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "mbox.h"
#include "sched.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define STACKSIZE 1024
#define THREAD_PRIORITY_MAIN 10
#define THREAD_PRIORITY_OTHER 7

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

void *thread_func(void *arg) {
    while (1) {
        // Simulate work
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                                          thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_OTHER, 0,
                                          thread_func, NULL, "thread2");

    xtimer_sleep(5);

    // Change thread priority
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_OTHER);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN);

    return 0;
}