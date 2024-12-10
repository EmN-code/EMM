
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_sub_t sub;
static sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;

static void publish_state(void)
{
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    // Register the topic if not already registered
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
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

int main(void)
{
    // Initialize the network stack
    puts("Initializing network stack...");
    if (netdev_default == NULL) {
        puts("Error: no default network device found");
        return 1;
    }
    netdev_default->driver->initialize(netdev_default);

    // Initialize the UDP socket for MQTT-SN
    puts("Initializing UDP socket...");
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 1883;
    if (sock_udp_create(&gw, &local, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    // Connect to the MQTT-SN broker
    puts("Connecting to MQTT-SN broker...");
    gw.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Set up periodic publishing
    puts("Starting periodic state publishing...");
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
