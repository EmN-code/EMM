
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define STATE_MSG "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET };
static emcute_topic_t topic;

void publish_state(void) {
    static char msg[10];
    strncpy(msg, STATE_MSG, sizeof(msg) - 1);
    msg[sizeof(msg) - 1] = '\0';

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Published state message");
    }
}

int main(void) {
    // Initialize the topic
    topic.name = TOPIC_NAME;
    topic.id = 0;  // Will be set by emcute_reg

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) <= 0) {
        puts("Error: invalid IP address");
        return 1;
    }
    gw.port = EMCUTE_PORT;

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    puts("Topic registered");

    // Run the emcute thread
    emcute_run(EMCUTE_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
