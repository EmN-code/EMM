// Required headers for networking and emcute
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/protnum.h"
#include "net/af.h"
#include "net/emcute.h"

// Constants
#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       (3U)
#define SERVER_ADDR     "47.102.103.1"
#define TOPIC_NAME      "state"
#define PUBLISH_INTERVAL (5 * US_PER_SEC)  // every five seconds

// Function for publishing data
static int publish_state(emcute_topic_t *topic) {
    const char *state_msg = "work"; // Message to publish
    int res = emcute_pub(topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);

    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
        return res;
    }

    printf("Published message: %s\n", state_msg);
    return EMCUTE_OK;
}

// Main function
env_t env;
int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    emcute_topic_t topic = { .name = TOPIC_NAME, .id = 0 };
    
    // Convert IP
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) == 0) {
        printf("Error: unable to parse gateway address\n");
        return 1;
    }
    
    // Connect to the broker
    printf("Connecting to broker at %s...\n", SERVER_ADDR);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Main loop
    while (1) {
        publish_state(&topic);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    // Disconnect
    emcute_discon();
    return 0;
}