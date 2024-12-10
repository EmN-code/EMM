// This code initializes an MQTT client on an ESP32 using the RIOT OS.
// It connects to the MQTT broker at 47.102.103.1:1883 and publishes the status of the ESP32 device every 5 seconds.

#include "thread.h"  
#include "net/emcute.h"  
#include "net/ipv6/addr.h"  
#include <string.h>
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "ESP32ClientID"
#define TOPIC_NAME "device/status"
#define INTERVAL (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

void *mqtt_pub_thread(void *arg) {
    char* status[] = {"work", "not work"};
    unsigned flags = EMCUTE_QOS_0;
    (void) arg;

    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", BROKER_IP, BROKER_PORT);
        return NULL;
    }
    printf("Connected to broker [%s]:%d\n", BROKER_IP, BROKER_PORT);

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        for(int i=0; i<2; i++) {
            if (emcute_pub(&topic, status[i], strlen(status[i]), flags) != EMCUTE_OK) {
                printf("Error: unable to publish data\n");
            } else {
                printf("Published status: %s\n", status[i]);
            }
            xtimer_sleep(INTERVAL);
        }
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  mqtt_pub_thread, NULL, "mqtt_pub_thread");
    return 0;
}