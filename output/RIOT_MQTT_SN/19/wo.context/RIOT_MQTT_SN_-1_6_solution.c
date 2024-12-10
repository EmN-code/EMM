#include <stdio.h>
#include <string.h>
#include "net/inet.h"
#include "net/sock/udp.h"
#include "emcute.h"
#include "xtimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32client"
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static emcute_topic_t topic;
static char *pub_topic = "device/state";
static char *state_message = "work";

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET6, MQTT_BROKER_IP, &gw.addr.ipv6);
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }
    printf("Connected to the broker at %s\n", MQTT_BROKER_IP);
    return 0;
}

static void publish_state(void) {
    topic.name = pub_topic;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }
    
    if (emcute_pub(&topic, state_message, strlen(state_message), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
        return;
    }
    printf("Published: %s on topic: %s\n", state_message, pub_topic);
}

int main(void) {
    if (connect_to_broker() != 0) {
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}