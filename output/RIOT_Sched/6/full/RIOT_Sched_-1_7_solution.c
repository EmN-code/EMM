#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)

static msg_t msg_queue[8];
static char thread_stack_one[STACKSIZE];
static char thread_stack_two[STACKSIZE];

static void* thread_func_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

static void* thread_func_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    kernel_pid_t thread_one_pid = thread_create(thread_stack_one, STACKSIZE, THREAD_PRIORITY_MAIN - 1, CREATE_STACKTEST, thread_func_one, NULL, "thread_one");
    kernel_pid_t thread_two_pid = thread_create(thread_stack_two, STACKSIZE, THREAD_PRIORITY_MAIN - 2, CREATE_STACKTEST, thread_func_two, NULL, "thread_two");

    // Changing priority at runtime
    ztimer_sleep(ZTIMER_MSEC, 5000);
    sched_set_status(thread_one_pid, THREAD_STATUS_PENDING);
    sched_set_status(thread_two_pid, THREAD_STATUS_PENDING);

    while(1) {
        // Main thread loop
        printf("Main thread running\n");
        ztimer_sleep(ZTIMER_MSEC, 2000);
    }

    return 0;
}