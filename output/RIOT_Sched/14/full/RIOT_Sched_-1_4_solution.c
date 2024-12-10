#include <unistd.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <thread_arch.h>
#include <time_units.h>
#include <event/thread.h>
#include <sched.h>
#include <architecture.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE_MAIN    1024
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP           "192.168.1.1"
#define MQTT_BROKER_PORT         1883

static char stack[2][THREAD_STACKSIZE_MAIN];
static kernel_pid_t pids[2];

void *thread_func(void *arg) {
    while (1) {
        printf("Thread %d running\n", (int)arg);
        usleep(1000000);
    }
    return NULL;
}

int NetworkConnect(Network *n, char* address_ip, int port_number) {
    // Simulated network connect function
    printf("Connecting to %s:%d\n", address_ip, port_number);
    return 0; // Simulating success
}

int main(void) {
    int rc;

    // Initialize and start two threads
    for (int i = 0; i < 2; i++) {
        pids[i] = thread_create(stack[i], THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, 0,
                                 thread_func, (void*)(intptr_t)i, "thread");
    }

    // Attempt to change the priority and connect to network
    while (1) {
        rc = NetworkConnect(NULL, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if(rc != 0) {
            printf("Connect failed, retrying...\n");
            usleep(1000000);
            continue;
        } else {
            printf("Connected successfully\n");
            break;
        }
    }

    // Change the priority of one of the threads
    sched_change_priority(thread_get(pids[0]), THREAD_PRIORITY_MAIN - 1);
    printf("Priority of thread 0 changed\n");

    while (1) {
        // Main thread loop
        printf("Main thread running\n");
        usleep(1000000);
    }

    return 0;
}
