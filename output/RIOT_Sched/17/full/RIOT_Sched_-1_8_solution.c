#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "stdint.h"
#include "sched.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "thread_arch.h"
#include "thread.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/emcute.h"

// Define the stack size for the threads
#define STACK_SIZE THREAD_STACKSIZE_DEFAULT

// Declare stacks for two threads
char thread_stack_one[STACK_SIZE];
char thread_stack_two[STACK_SIZE];

// Function for the first thread
void *thread_func_one(void *arg) {
    while(1) {
        // Simulate some work in the first thread
    }
    return NULL;
}

// Function for the second thread
void *thread_func_two(void *arg) {
    while(1) {
        // Simulate some work in the second thread
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;

    // Create the first thread
    pid_one = thread_create(thread_stack_one, STACK_SIZE,
                            THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST,
                            thread_func_one, NULL, "thread_one");

    // Create the second thread
    pid_two = thread_create(thread_stack_two, STACK_SIZE,
                            THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST,
                            thread_func_two, NULL, "thread_two");

    // Change the priority of the first thread
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 3);

    // Change the priority of the second thread
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 4);

    return 0;
}