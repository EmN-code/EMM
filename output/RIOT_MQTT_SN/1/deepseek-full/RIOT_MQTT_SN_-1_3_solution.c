
#include <thread.h>
#include <net/sock/udp.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <stdlib.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <stdio.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"

static sock_udp_ep_t broker_ep = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    const char *message = STATE_MESSAGE;
    int res = emcute_pub(&topic, message, strlen(message), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    } else {
        printf("State published: %s\n", message);
    }
}

int main(void) {
    // Initialize the UDP endpoint for the broker
    inet_pton(AF_INET, BROKER_ADDR, &broker_ep.addr.ipv4);

    // Connect to the MQTT-SN broker
    int res = emcute_con(&broker_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
