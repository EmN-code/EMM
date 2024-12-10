
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define TOPIC_NAME "state"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET };

void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published state: %s\n", state);
    }
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: unable to initialize the network stack");
        return 1;
    }

    // Convert the broker IP address from string to binary form
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }
    gw.port = BROKER_PORT;

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the MQTT-SN broker");
        return 1;
    }
    puts("Connected to the MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register the topic");
        return 1;
    }
    printf("Registered topic: %s with ID: %u\n", topic.name, topic.id);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();
    puts("Disconnected from the MQTT-SN broker");

    return 0;
}
