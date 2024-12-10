
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = BROKER_PORT, .family = AF_INET };

void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    sock_udp_t sock;
    ipv4_addr_t broker_addr;

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert the broker IP address from string to binary form
    if (ipv4_addr_from_str(&broker_addr, BROKER_IP) == NULL) {
        puts("Error parsing broker IP address");
        return 1;
    }

    // Set the broker address in the endpoint structure
    gw.addr.ipv4 = broker_addr;

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    // Disconnect from the broker
    emcute_discon();

    return 0;
}
