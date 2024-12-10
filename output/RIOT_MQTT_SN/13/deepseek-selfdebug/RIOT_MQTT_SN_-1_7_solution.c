
#include <stdio.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static char stack[THREAD_STACKSIZE_DEFAULT];

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static int state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC;
    topic.id = 0; // Topic ID will be assigned by the broker

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    // Publish the state
    if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
        return;
    }

    // Toggle state
    state_index = 1 - state_index;
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = BROKER_PORT;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Run the emcute thread
    emcute_run(BROKER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
