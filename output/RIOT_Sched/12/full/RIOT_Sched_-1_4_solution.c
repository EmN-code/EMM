#include "thread_config.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "unistd.h"
#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "sched.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 10

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        // Implement actual thread functionality here
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread_func, NULL, "thread2");

    if (thread1_pid < 0 || thread2_pid < 0) {
        printf("Error: Thread creation failed\n");
        return 1;
    }

    // Change thread priority at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 4);

    return 0;
}