#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread_arch.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_system.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "time_units.h"

#ifndef SCHED_PRIO_LEVELS
#define SCHED_PRIO_LEVELS 16
#endif

#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY_MAIN (SCHED_PRIO_LEVELS - 1)
#define ADDR_IP "127.0.0.1"
#define PORT_NUMBER 1883

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t thread1;
kernel_pid_t thread2;

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        // Simulate some work
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

int main(void) {
    // Create two threads
    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread2");
    
    // Change priority at runtime
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 1);

    // Simulate network connection with retry logic
    Network network;
    int rc;
    while (1) {
        rc = NetworkConnect(&network, ADDR_IP, PORT_NUMBER);
        if (rc != 0) {
            xtimer_sleep(1);
            printf("Network connection failed, retrying...\n");
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    // Keep the main thread alive
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return 0;
}