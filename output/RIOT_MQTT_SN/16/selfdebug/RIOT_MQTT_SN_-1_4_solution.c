#include <stdio.h>
#include <stdlib.h>
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "ztimer.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC       "state"
#define MQTT_DATA_WORK   "work"
#define MQTT_DATA_NOWORK "not work"
#define PUBLISH_INTERVAL 5

// Function to initialize the MQTT-SN connection
static int mqtt_sn_init(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    return emcute_con(&gw, true, NULL, NULL, 0, 0);
}

// Function to publish a message
static int mqtt_sn_pub(const char* data) {
    emcute_topic_t topic;
    topic.name = MQTT_TOPIC;
    emcute_reg(&topic);
    return emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0);
}

// Main function to periodically publish MQTT-SN messages
int main(void) {
    printf("MQTT-SN client example started\n");
    if (mqtt_sn_init() != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker\n");
        return 1;
    }
    while (1) {
        mqtt_sn_pub(MQTT_DATA_WORK);
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * 1000);
    }
    return 0;
}