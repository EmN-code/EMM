#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread_config.h"
#include "thread.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "netemcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "unistd.h"
#include "MQTTClient.h"

#define STACKSIZE 1024
#define THREAD_PRIORITY_DEFAULT 5

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

void *thread_handler1(void *arg) {
    kernel_pid_t pid = thread_getpid();
    fprintf(stderr, "Thread %d: Started with priority %d\n", pid, thread_get_priority(thread_get(pid)));
    sched_change_priority(thread_get(pid), THREAD_PRIORITY_DEFAULT + 2);
    fprintf(stderr, "Thread %d: Priority changed to %d\n", pid, thread_get_priority(thread_get(pid)));
    while (1) {}
    return NULL;
}

void *thread_handler2(void *arg) {
    kernel_pid_t pid = thread_getpid();
    fprintf(stderr, "Thread %d: Started with priority %d\n", pid, thread_get_priority(thread_get(pid)));
    sched_change_priority(thread_get(pid), THREAD_PRIORITY_DEFAULT + 1);
    fprintf(stderr, "Thread %d: Priority changed to %d\n", pid, thread_get_priority(thread_get(pid)));
    while (1) {}
    return NULL;
}

int main(void) {
    thread_create(stack1, STACKSIZE, THREAD_PRIORITY_DEFAULT, 0, thread_handler1, NULL, "thread1");
    thread_create(stack2, STACKSIZE, THREAD_PRIORITY_DEFAULT, 0, thread_handler2, NULL, "thread2");
    while (1) {}
    return 0;
}