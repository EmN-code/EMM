
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
static sock_udp_t sock;

static void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the gateway

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
        puts("State published successfully");
    }

    // Toggle state
    state_index = (state_index + 1) % 2;
}

int main(void) {
    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();
    puts("Disconnected from MQTT-SN broker");

    return 0;
}
