#include <string.h>  // For string operations
#include <stdio.h>    // For standard I/O functions
#include <arpa/inet.h> // For inet_pton function
#include <net/sock/udp.h>  // For UDP socket functions
#include <stdlib.h>   // For standard library functions

#include "net/emcute.h"  // For emcute functions

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define STATE_TOPIC     "device/state"
#define PUBLISH_INTERVAL (5000U)  // milliseconds

static emcute_topic_t state_topic;
static sock_udp_ep_t remote;

static void init_mqtt(void) {
    remote.family = AF_INET;
    remote.port = EMCUTE_PORT;
    if(inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Could not convert server address\n");
        exit(1);
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Could not connect to MQTT broker\n");
        exit(1);
    }

    state_topic.name = STATE_TOPIC;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Error: Could not register topic\n");
        exit(1);
    }
}

int main(void) {
    char payload[64];
    printf("Starting MQTT-SN example\n");
    init_mqtt();

    while (1) {
        snprintf(payload, sizeof(payload), "work");
        if (emcute_pub(&state_topic, payload, strlen(payload), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: Could not publish message\n");
        }

        xtimer_sleep(PUBLISH_INTERVAL / 1000);  // Convert milliseconds to seconds
    }

    return 0;
}