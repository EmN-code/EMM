
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
static emcute_topic_t topic;

static void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    printf("Publishing state: %s\n", state);
    emcute_pub(&topic, state, strlen(state), 0);
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = { .family = AF_INET, .port = 0 };

    // Initialize UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }
    printf("Registered topic: %s with ID: %u\n", topic.name, topic.id);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from MQTT-SN broker
    emcute_discon();
    puts("Disconnected from MQTT-SN broker");

    return 0;
}
