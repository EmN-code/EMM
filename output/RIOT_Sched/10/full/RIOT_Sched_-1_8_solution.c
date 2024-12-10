#include "xtimer.h"
#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "thread_arch.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define THREAD_ONE_STACKSIZE (1 << 10)
#define THREAD_TWO_STACKSIZE (1 << 10)
#define THREAD_ONE_PRIORITY  (5)
#define THREAD_TWO_PRIORITY  (6)

char thread_one_stack[THREAD_ONE_STACKSIZE];
char thread_two_stack[THREAD_TWO_STACKSIZE];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread One is running");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread Two is running");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                   THREAD_ONE_PRIORITY, THREAD_CREATE_STACKTEST,
                                   thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                   THREAD_TWO_PRIORITY, THREAD_CREATE_STACKTEST,
                                   thread_two, NULL, "thread_two");
    
    // Change the priority of the threads
    sched_change_priority(thread_get(thread_one_pid), 4);
    sched_change_priority(thread_get(thread_two_pid), 3);

    // Main thread yielding
    while(1) {
        xtimer_sleep(2);
        puts("Main thread yielding");
    }

    return 0;
}