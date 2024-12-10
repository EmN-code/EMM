// Include required headers
#include <stdio.h>
#include <string.h>
#include "net/ipv6/addr.h"
#include "net/emcute.h"

// MQTT-SN broker address
#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     1883
#define PUBLISH_INTERVAL (5U)  // publish interval in seconds

// MQTT topic and message
#define TOPIC_NAME      "device/state"
#define MESSAGE_WORK    "work"
#define MESSAGE_NOT_WORK "not work"

// Function prototypes
static int _publish_state(emcute_topic_t *topic, const char *message);

int main(void) {
    // Initialize MQTT-SN connection variables
    emcute_topic_t topic;
    sock_udp_ep_t gw;
    gw.port = SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_IP);

    // Initialize a connection to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker.\n");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic.\n");
        return 1;
    }

    // Publish state regularly
    while (1) {
        // Publish the working state
        if (_publish_state(&topic, MESSAGE_WORK) != EMCUTE_OK) {
            printf("Error: unable to publish state.\n");
        }

        // Sleep for PUBLISH_INTERVAL
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    // Clean up by disconnecting from the broker
    emcute_discon();
    return 0;
}

static int _publish_state(emcute_topic_t *topic, const char *message) {
    int res = emcute_pub(topic, message, strlen(message), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message. EMCUTE error %d\n", res);
    }
    return res;
}