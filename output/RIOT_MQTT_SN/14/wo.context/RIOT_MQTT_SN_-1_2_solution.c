#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID      ("esp32_client")
#define EMCUTE_PORT    (1883)
#define MQTT_TOPIC     ("device/state")
#define BROKER_IP      "47.102.103.1"

static emcute_topic_t mqtt_topic;

void publish_state(void) {
    const char *state = "work or not work";
    int res = emcute_pub(&mqtt_topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error while publishing: %d\n", res);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Connect to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    // Registering the topic
    mqtt_topic.name = MQTT_TOPIC;
    if (emcute_reg(&mqtt_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Publish the state every five seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}