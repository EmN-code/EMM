
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MESSAGE "work"

static emcute_sub_t sub;
static sock_udp_ep_t gw;

void publish_state(void) {
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: could not register topic\n");
        return;
    }

    res = emcute_pub(&topic, MESSAGE, strlen(MESSAGE), 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not publish message\n");
    } else {
        printf("Published: %s\n", MESSAGE);
    }
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        printf("Error: cannot initialize network stack\n");
        return 1;
    }

    // Set up the gateway endpoint
    memset(&gw, 0, sizeof(gw));
    gw.family = AF_INET;
    gw.port = MQTT_BROKER_PORT;
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) <= 0) {
        printf("Error: invalid broker IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not connect to MQTT-SN broker\n");
        return 1;
    }

    // Run the emcute thread
    emcute_run(MQTT_BROKER_PORT, MQTT_CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
