#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "net/sock/util.h"
#include "architecture.h"
#include "netinet/in.h"
#include "sched.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "stdio.h"
#include "msg.h"
#include "stdlib.h"
#include "thread_config.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "mutex.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "thread.h"
#include "thread_arch.h"
#include "arpa/inet.h"
#include "timex.h"
#include "esp_system.h"
#include "sys/socket.h"
#include "net/gcoap.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_STACKSIZE_SMALL 512
#define THREAD_PRIORITY_MAIN (thread_get_priority(thread_get(0)) - 1)
#define THREAD_PRIORITY_SECOND (thread_get_priority(thread_get(0)))

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_SMALL];

void *thread_one(void *arg) {
    while (1) {
        xtimer_sleep(1);
        puts("Thread One Running");
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        xtimer_sleep(1);
        puts("Thread Two Running");
    }
    return NULL;
}

int main(void) {
    /* Disable interrupts before changing priority */
    irq_disable();
    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN,
                                         THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    kernel_pid_t pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_SECOND,
                                         THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");
    /* Change priority of thread_two */
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 1);
    /* Enable interrupts back */
    irq_enable();
    return 0;
}