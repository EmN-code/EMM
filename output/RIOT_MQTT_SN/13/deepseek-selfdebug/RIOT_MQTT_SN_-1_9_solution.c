
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;
    const char *state = states[state_index];
    state_index = (state_index + 1) % 2; // Toggle between "work" and "not work"

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published state: %s\n", state);
    }
}

int main(void) {
    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("Error: no network device found");
        return 1;
    }

    // Initialize the EMCUTE stack
    emcute_run(BROKER_PORT, CLIENT_ID);

    // Set up the gateway endpoint
    gw.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
