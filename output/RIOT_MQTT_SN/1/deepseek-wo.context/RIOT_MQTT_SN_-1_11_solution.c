
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"

static emcute_sub_t sub;
static sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the gateway

    // Register the topic if not already registered
    if (topic.id == 0) {
        if (emcute_reg(&topic) != EMCUTE_OK) {
            puts("Error: unable to register topic");
            return;
        }
    }

    // Publish the state
    if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        puts("State published");
    }

    // Toggle state
    state_index = 1 - state_index;
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: unable to initialize network stack");
        return 1;
    }

    // Set up the gateway endpoint
    gw.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: unable to parse server IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    // Run the emCute thread
    emcute_run(SERVER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
