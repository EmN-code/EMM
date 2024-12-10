#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "sched.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "architecture.h"
#include "unistd.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MY_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define MY_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_MAIN (5U)  // Defined value for simulation purposes

char stack1[MY_STACKSIZE];
char stack2[MY_STACKSIZE];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread work simulation
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, MY_STACKSIZE, MY_PRIORITY, 0, thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, MY_STACKSIZE, THREAD_PRIORITY_MAIN, 0, thread_func, NULL, "thread2");

    // Change priority of thread1 during runtime
    thread_t *t1 = thread_get(thread1);
    sched_change_priority(t1, THREAD_PRIORITY_MAIN - 2);

    while (1) {
        // Main loop work
    }
    
    return 0;
}