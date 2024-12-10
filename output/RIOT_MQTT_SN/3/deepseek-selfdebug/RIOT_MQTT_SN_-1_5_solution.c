
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[20];
static emcute_topic_t topic;

void publish_state(void) {
    static int state = 0;
    const char *states[] = {"work", "not work"};

    // Toggle state
    state = !state;

    // Prepare the message
    strncpy(state_msg, states[state], sizeof(state_msg) - 1);
    state_msg[sizeof(state_msg) - 1] = '\0';

    // Publish the message
    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    emcute_sub_t sub;

    // Initialize the message queue
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    // Set up the gateway endpoint
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Start the periodic publishing
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the broker
    emcute_discon();
    puts("Disconnected from MQTT-SN broker");

    return 0;
}
