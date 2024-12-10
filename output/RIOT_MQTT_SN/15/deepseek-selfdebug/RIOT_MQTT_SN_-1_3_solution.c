
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include "emcute.h"

#define TOPIC_NAME "state"
#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static int state_index = 0;

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
    state_index = (state_index + 1) % 2;
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV4_EP_ANY;
    gw.port = MQTT_SERVER_PORT;

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, MQTT_SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    // Run the EMCUTE thread
    emcute_run(MQTT_SERVER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
