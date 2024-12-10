#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread_arch.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "architecture.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "paho_mqtt.h"
#include "stddef.h"
#include "thread_config.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIO1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIO2 (THREAD_PRIORITY_MAIN - 2)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_task1(void *arg) {
    (void)arg;
    while(1) {
        puts("Thread 1 running");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_task2(void *arg) {
    (void)arg;
    while(1) {
        puts("Thread 2 running");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIO1, 0, thread_task1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIO2, 0, thread_task2, NULL, "thread2");

    if (thread1 != KERNEL_PID_UNDEF && thread2 != KERNEL_PID_UNDEF) {
        puts("Threads created successfully");

        // Change priority of thread1
        sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN);

        // Change priority of thread2
        sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN + 1);
    }
    else {
        puts("Error: Thread creation failed");
    }

    return 0;
}