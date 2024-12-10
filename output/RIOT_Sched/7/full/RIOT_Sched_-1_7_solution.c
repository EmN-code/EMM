#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread_arch.h>
#include <thread.h>
#include <thread_config.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <netinet/in.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <architecture.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <event/thread.h>
#include <msg.h>
#include <net/gcoap.h>

#define STACK_SIZE 1024
#define PRIORITY 5
#define SECOND_PRIORITY 10

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one_func(void *arg) {
    (void)arg;
    while(1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    (void)arg;
    while(1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Initialize thread_one
    thread_one_pid = thread_create(stack1, STACK_SIZE, PRIORITY, CREATE_STACKTEST, thread_one_func, NULL, "thread_one");

    // Initialize thread_two
    thread_two_pid = thread_create(stack2, STACK_SIZE, SECOND_PRIORITY, CREATE_STACKTEST, thread_two_func, NULL, "thread_two");

    // Change the priority of thread_one after some time
    xtimer_sleep(5);
    printf("Changing priority of thread one\n");
    sched_change_priority(thread_get(thread_one_pid), SECOND_PRIORITY);

    return 0;
}