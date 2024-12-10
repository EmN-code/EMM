#include "ztimer.h"
#include "xtimer.h"
#include "thread_arch.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "thread_config.h"

#define STATUS_STACK_SIZE (THREAD_STACKSIZE_MAIN * 4)
#define CONTROL_STACK_SIZE (THREAD_STACKSIZE_MAIN * 4)

static char stack1[STATUS_STACK_SIZE];
static char stack2[CONTROL_STACK_SIZE];

static void *thread1_handler(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 1 running");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

static void *thread2_handler(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 2 running");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1), 8, THREAD_CREATE_STACKTEST, thread1_handler, NULL, "thread1");
    kernel_pid_t thread2_pid = thread_create(stack2, sizeof(stack2), 9, THREAD_CREATE_STACKTEST, thread2_handler, NULL, "thread2");

    if (thread1_pid < 0 || thread2_pid < 0) {
        puts("Error: unable to create threads");
        return 1;
    }
    
    ztimer_sleep(ZTIMER_MSEC, 2000);
    
    sched_change_priority(thread_get(thread1_pid), 10);
    sched_change_priority(thread_get(thread2_pid), 7);

    ztimer_sleep(ZTIMER_MSEC, 10000);
    return 0;
}