
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "sched.h"
#include "xtimer.h"
#include "thread.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_LOW, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    ztimer_sleep(ZTIMER_MSEC, 5000);

    sched_change_priority(thread_get(pid1), PRIORITY_HIGH);
    sched_change_priority(thread_get(pid2), PRIORITY_LOW);

    ztimer_sleep(ZTIMER_MSEC, 5000);

    sched_change_priority(thread_get(pid1), PRIORITY_LOW);
    sched_change_priority(thread_get(pid2), PRIORITY_HIGH);

    return 0;
}
