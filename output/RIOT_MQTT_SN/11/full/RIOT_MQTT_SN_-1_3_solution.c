#include <stdlib.h>
#include <stdio.h>
#include <thread.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <sys/socket.h>
// Ensure that the necessary headers are included for RIOT and network functionalities
#include "MQTTClient.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MECUTE_PRIO      (THREAD_PRIORITY_MAIN - 1)
#define BUF_SIZE         (128U)
#define MQTT_TOPIC       "device/state"

static char mqtt_thread_stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t mqtt_thread_pid;

void *mqtt_publisher(void *arg) {
    (void)arg;
    emcute_topic_t t;
    t.name = MQTT_TOPIC;
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP);

    int tries = 0;
    while (1) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            printf("Error: unable to connect to [%s]:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
            tries++;
            if (tries > 5) {
                printf("Maximum retry attempts reached.\n");
                return NULL;
            }
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        t.id = 0;  // Must be set to 0 before publishing

        while (1) {
            if (emcute_pub(&t, "work", 5, EMCUTE_QOS_0) != EMCUTE_OK) {
                printf("Pub failed\n");
            } else {
                printf("Published: work\n");
            }
            ztimer_sleep(ZTIMER_MSEC, 5000);
        }
    }
    return NULL;
}

int main(void) {
    puts("MQTT-SN publisher example\n");

    /* Initialize the MQTT publisher thread */
    mqtt_thread_pid = thread_create(
        mqtt_thread_stack, sizeof(mqtt_thread_stack),
        MECUTE_PRIO, THREAD_CREATE_STACKTEST,
        mqtt_publisher, NULL, "mqtt_publisher"
    );

    if (mqtt_thread_pid <= KERNEL_PID_UNDEF) {
        puts("Error initializing the MQTT thread\n");
        return 1;
    }

    return 0;
}