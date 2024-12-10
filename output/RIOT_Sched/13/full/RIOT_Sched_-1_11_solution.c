#include <xtimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread_arch.h>
#include <thread_config.h>
#include <paho_mqtt.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <esp_wifi.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <ztimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <time_units.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <architecture.h>
#include <kernel_defines.h>
#include <MQTTClient.h>
#include <stdatomic.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

#define STACKSIZE 1024
#define PRIORITY 5
#define MQTT_BROKER_IP "192.168.1.1"
#define MQTT_BROKER_PORT 1883

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;
static char thread_one_stack[STACKSIZE];
static char thread_two_stack[STACKSIZE];
static mqtt_client_t mqtt_client;

static void *thread_one(void *arg) {
    (void)arg;
    while(1) {
        // Placeholder for realistic thread function
    }
    return NULL;
}

static void *thread_two(void *arg) {
    (void)arg;
    while(1) {
        // Placeholder for realistic thread function
    }
    return NULL;
}

static int reconnect_network() {
    int rc;
    Network network;

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc == 0) {
            printf("Successfully connected to network\n");
            break;
        } else {
            printf("Failed to connect, retrying...\n");
            xtimer_sleep(1);
        }
    }
    return rc;
}

int main(void) {
    puts("ESP32 RIOT MQTT Example\n");

    thread_one_pid = thread_create(thread_one_stack, STACKSIZE, PRIORITY,
                                   THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(thread_two_stack, STACKSIZE, PRIORITY,
                                   THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    if (reconnect_network() != 0) {
        puts("Network initialization failed, halting example\n");
        return 1;  // Return failure error code
    }

    while (1) {
        thread_yield_higher();
    }

    return 0;
}