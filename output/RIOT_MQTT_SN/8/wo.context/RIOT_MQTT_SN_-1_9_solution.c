#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <net/emcute.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STATE_MSG_SIZE 16 // Size enough to hold "work" or "not work"

static emcute_topic_t topic;

void publish_state(const char *state) {
    printf("Publishing: %s\n", state);
    int ret = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (ret != EMCUTE_OK) {
        printf("Error: unable to publish data (%d)\n", ret);
    }
}

int main(void) {
    printf("Starting MQTT-SN application with emcute\n");

    // Setting up the broker address
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    // Register a topic, assume "state/topic"
    topic.name = "state/topic";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Publish state every 5 seconds
    while(1) {
        publish_state("work");
        xtimer_sleep(5);
        publish_state("not work");
        xtimer_sleep(5);
    }

    // Clean up and disconnect
    emcute_discon();
    return 0;
}