
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/inet_pton.h>
#include <emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MSG "work"

static sock_udp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };

static void publish_state(void) {
    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0; // Topic ID will be assigned by the gateway

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: could not register topic (%d)\n", res);
        return;
    }

    res = emcute_pub(&topic, MSG, strlen(MSG), 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not publish message (%d)\n", res);
    } else {
        printf("Published: %s\n", MSG);
    }
}

int main(void) {
    // Initialize the network stack
    net_init();

    // Convert the broker IP address to binary form
    if (inet_pton(AF_INET, BROKER_IP, &broker.addr.ipv4) <= 0) {
        printf("Error: invalid broker IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&broker, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: could not connect to MQTT-SN broker (%d)\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();

    return 0;
}
