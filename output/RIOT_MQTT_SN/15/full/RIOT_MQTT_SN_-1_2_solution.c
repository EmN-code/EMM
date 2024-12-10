#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/util.h"

#define EMCUTE_PORT (1883U)
#define MQTT_BROKER_IP "47.102.103.1"
#define TOPIC_NAME "device/state"

static sock_udp_ep_t broker_endpoint = {
    .family = AF_INET6,
    .port = EMCUTE_PORT
};

int main(void) {
    if (ipv6_addr_from_str((ipv6_addr_t *)&broker_endpoint.addr.ipv6, MQTT_BROKER_IP) == NULL) {
        printf("Error: unable to parse IPv6 address\n");
        return -1;
    }

    emcute_topic_t topic;
    unsigned flags = EMCUTE_QOS_0;
    topic.name = TOPIC_NAME;

    int ret;

    char *state_msg = "work";  // Initial state is "work"

    // Continuously try to connect to the gateway
    while (1) {
        ret = emcute_con(&broker_endpoint, true, NULL, NULL, 0, 0);
        if (ret == EMCUTE_OK) {
            printf("Successfully connected to gateway\n");
            break;
        }
        else {
            printf("Failed to connect to gateway: retrying in 1 second\n");
            sleep(1);  // Retry every 1 second
        }
    }

    // Register topic
    ret = emcute_reg(&topic);
    if (ret != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return -1;
    }

    while (1) {
        // Publish message
        ret = emcute_pub(&topic, state_msg, strlen(state_msg), flags);
        if (ret != EMCUTE_OK) {
            printf("Failed to publish message\n");
        } else {
            printf("Published: %s\n", state_msg);
        }

        // Sleep for 5 seconds
        sleep(5);
    }

    return 0;
}