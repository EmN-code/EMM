
#include <stdio.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static int state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2; // Toggle between "work" and "not work"

    printf("Publishing state: %s\n", state);
    emcute_pub(&topic, state, strlen(state), 0);
}

int main(void) {
    // Initialize the MQTT-SN topic
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the gateway

    // Initialize the gateway endpoint
    gw.family = AF_INET;
    gw.port = MQTT_SN_BROKER_PORT;
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic with the broker
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