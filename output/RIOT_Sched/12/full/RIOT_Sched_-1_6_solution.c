#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_MAIN    (5)
#define THREAD_PRIORITY_HIGH    (3)

char stack_one[THREAD_STACKSIZE_DEFAULT];
char stack_two[THREAD_STACKSIZE_DEFAULT];
kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one_function(void *arg) {
    (void)arg; // Avoid unused parameter warning
    while (1) {
        // Simulate work
        usleep(1000000);
        printf("Thread One is running\n");
    }
    return NULL;
}

void *thread_two_function(void *arg) {
    (void)arg; // Avoid unused parameter warning
    while (1) {
        // Simulate work
        usleep(1000000);
        printf("Thread Two is running\n");
    }
    return NULL;
}

int main(void) {
    printf("Starting threads example\n");
    
    thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN, 0, thread_one_function, NULL, "thread_one");
    thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_HIGH, 0, thread_two_function, NULL, "thread_two");

    // Changing thread priorities at runtime
    thread_yield();
    
    printf("Changing priority of Thread One\n");
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_HIGH);

    while(1) { // Keep the main thread running
        usleep(1000000);
    }

    return 0;
}