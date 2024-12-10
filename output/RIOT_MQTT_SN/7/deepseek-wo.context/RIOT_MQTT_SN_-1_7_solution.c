
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/emcute.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0; // Topic ID will be assigned by the broker

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return;
    }

    // Publish the state
    if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), 0) != EMCUTE_OK) {
        puts("Error: could not publish message");
    } else {
        printf("Published: %s\n", states[state_index]);
    }

    // Toggle state
    state_index = (state_index + 1) % 2;
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: could not initialize network stack");
        return 1;
    }

    // Set up the MQTT-SN broker endpoint
    sock_udp_ep_t broker = SOCK_IPV4_EP_ANY;
    broker.port = MQTT_BROKER_PORT;
    broker.family = AF_INET;

    // Convert the IP address string to binary form
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4) != 1) {
        puts("Error: could not convert IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&broker, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return 1;
    }

    // Run the EMCUTE thread
    emcute_run(MQTT_BROKER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
