#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "sched.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "architecture.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "stdio.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_DEFAULT - 1)
#define THREAD_PRIORITY_SECOND (THREAD_PRIORITY_DEFAULT - 2)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

static void *thread1_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

static void *thread2_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                                THREAD_CREATE_STACKTEST, thread1_handler, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_SECOND,
                                THREAD_CREATE_STACKTEST, thread2_handler, NULL, "thread2");

    printf("Threads successfully started\n");
    xtimer_sleep(3);

    printf("Changing priority of Thread 1\n");
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_SECOND);

    return 0;
}