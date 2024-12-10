#include "thread_config.h"
#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <arpa/inet.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

char thread_stack[THREAD_STACKSIZE];
kernel_pid_t thread_pid;

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        // Simulate some work in the thread
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    // Create a thread with default priority
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN + 1,
                               CREATE_STACKTEST, thread_handler, NULL, "example_thread");
    if (thread_pid < 0) {
        printf("Thread creation failed\n");
        return 1;
    }

    // Change the priority of the created thread
    sched_change_priority(thread_get(thread_pid), THREAD_PRIORITY_MAIN - 1);
    
    while (1) {
        // Simulate main work
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}