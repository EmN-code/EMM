
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define MSG_INTERVAL 5000000  // 5 seconds in microseconds

static char state[] = "work";

void publish_state(void) {
    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return;
    }

    res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    printf("Connected to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    emcute_discon();
    return 0;
}
