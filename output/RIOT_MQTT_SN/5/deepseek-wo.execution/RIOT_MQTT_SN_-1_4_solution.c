
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <stdbool.h>
#include <mma8x5x.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdatomic.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_regs.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MSG "work"

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = SOCK_IPV6_EP_ANY;

void publish_state(void) {
    char msg[] = STATE_MSG;
    int res;

    res = emcute_pub(&topic, msg, sizeof(msg) - 1, 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void) {
    sock_udp_t sock;
    int res;

    // Initialize the UDP socket
    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    // Set the gateway endpoint
    gw_ep.family = AF_INET;
    gw_ep.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_ADDR, &gw_ep.addr.ipv4);

    // Connect to the MQTT-SN broker
    res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        thread_sleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }

    return 0;
}
