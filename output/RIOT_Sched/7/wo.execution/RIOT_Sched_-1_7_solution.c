#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "sched.h"
#include "netinet/in.h"
#include "net_gcoap.h"
#include "net_emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define THREAD_PRIORITY_MAIN 5
#define THREAD_STACKSIZE_MAIN 1024

static char stack_one[THREAD_STACKSIZE_MAIN];
static char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    while (1) {
        printf("Running thread one\n");
        xtimer_sleep(1);
        sched_change_priority((thread_getpid()+1)%2, THREAD_PRIORITY_MAIN + 1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        printf("Running thread two\n");
        xtimer_sleep(1);
        sched_change_priority((thread_getpid()+1)%2, THREAD_PRIORITY_MAIN);
    }
    return NULL;
}

int main(void) {
    puts("Starting threads...");

    thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN, 0, thread_one, NULL, "thread_one");
    thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 1, 0, thread_two, NULL, "thread_two");

    while (1) {
        // main thread can perform other tasks
        xtimer_sleep(1);
    }

    return 0;
}