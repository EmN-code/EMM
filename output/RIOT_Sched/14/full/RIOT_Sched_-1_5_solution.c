#include "xtimer.h"
#include "timex.h"
#include <stdbool.h>
#include "sys/socket.h"
#include "netinet/in.h"
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time_units.h>
#include <stddef.h>
#include <stdatomic.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <architecture.h>
#include <event/thread.h>
#include <thread_arch.h>
#include <kernel_defines.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <unistd.h>
#include "Network.h" // Assuming "Network" is related to the network connection.

#define MQTT_BROKER_IP   "192.168.1.100"
#define MQTT_BROKER_PORT 1883
#define THREAD_STACKSIZE_MAIN    (1024)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread_pid1;
kernel_pid_t thread_pid2;

void *thread1_handler(void *arg) {
    while (1) {
        printf("Thread 1 running.\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    while (1) {
        printf("Thread 2 running.\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    printf("Starting RIOT application on ESP32...\n");

    thread_pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread1_handler, NULL, "thread1");
    if (thread_pid1 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread1\n");
        return EXIT_FAILURE;
    }

    thread_pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread2_handler, NULL, "thread2");
    if (thread_pid2 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread2\n");
        return EXIT_FAILURE;
    }

    // Change the priority of the first thread on runtime
    sched_change_priority(thread_pid1, THREAD_PRIORITY_MAIN - 1);
    printf("Changed priority of thread1\n");
    
    // Network and MQTT setup loop with extensive error handling
    Network network;
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }

    while (1) {
        // Main loop
    }

    return 0;
}