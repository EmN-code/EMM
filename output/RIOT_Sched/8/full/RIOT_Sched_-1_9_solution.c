#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>

#include "paho_mqtt.h"
#include "msg.h"
#include "kernel_defines.h"
#include "sched.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "evtimer_msg.h"

#define THREAD_PRIORITY_MAIN 1
#define THREAD_PRIORITY_SECOND 2
#define THREAD_CREATE_STACKTEST 0

#define MQTT_BROKER_IP "192.168.1.1"
#define MQTT_BROKER_PORT 1883

char stack_one[KERNEL_CONF_STACK_SIZE_DEFAULT];
char stack_two[KERNEL_CONF_STACK_SIZE_DEFAULT];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

static void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One is working\n");
        
        // Simulate work
        xtimer_sleep(2);
    }
    return NULL;
}

static void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two is working\n");
        
        // Simulate work
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    printf("Starting two threads\n");

    // Create two threads
    thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");
    thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_SECOND, THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    // Change the priority of the threads at runtime
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_SECOND);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN);

    while (1) {
        
        // Simulate main process work
        xtimer_sleep(5);
    }

    return 0;
}
