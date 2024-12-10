
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define BROKER_IP "47.102.103.1"
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .port = EMCUTE_PORT };

void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    // Register the topic
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

int main(void) {
    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("Error: no network devices available");
        return 1;
    }

    // Convert the broker IP address
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Run the EMCUTE thread
    emcute_run(EMCUTE_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
